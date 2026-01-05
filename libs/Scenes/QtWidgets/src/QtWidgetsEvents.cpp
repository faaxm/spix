/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtWidgetsEvents.h"

#include <QtWidgetsItem.h>
#include <QtWidgetsItemTools.h>

#include <Spix/Data/PasteboardContent.h>

#include <QApplication>
#include <QObject>
#include <QWidget>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

namespace spix {

namespace {

QWidget* getWidgetAndPositionForItem(Item* item, Point relToItemPos, QPoint& widgetPos)
{
    auto qtitem = dynamic_cast<QtWidgetsItem*>(item);
    if (!qtitem)
        return nullptr;

    QWidget* widget = qtitem->qwidget();
    widgetPos = QPoint(static_cast<int>(relToItemPos.x), static_cast<int>(relToItemPos.y));

    // Find the actual child widget at this position for proper event delivery
    QWidget* childAtPos = widget->childAt(widgetPos);
    if (childAtPos) {
        // Map position to child widget coordinates
        widgetPos = childAtPos->mapFrom(widget, widgetPos);
        return childAtPos;
    }

    return widget;
}

Qt::MouseButton getQtMouseButtonValue(MouseButton button)
{
    unsigned qtButton = Qt::MouseButton::NoButton;

    if (button & MouseButtons::Left) {
        qtButton |= Qt::MouseButton::LeftButton;
    }

    if (button & MouseButtons::Right) {
        qtButton |= Qt::MouseButton::RightButton;
    }

    if (button & MouseButtons::Middle) {
        qtButton |= Qt::MouseButton::MiddleButton;
    }

    return static_cast<Qt::MouseButton>(qtButton);
}

Qt::KeyboardModifiers getQtKeyboardModifiers(KeyModifier mod)
{
    Qt::KeyboardModifiers qtmod = Qt::NoModifier;

    if (mod & KeyModifiers::Shift) {
        qtmod = qtmod | Qt::ShiftModifier;
    }
    if (mod & KeyModifiers::Control) {
        qtmod = qtmod | Qt::ControlModifier;
    }
    if (mod & KeyModifiers::Alt) {
        qtmod = qtmod | Qt::AltModifier;
    }
    if (mod & KeyModifiers::Meta) {
        qtmod = qtmod | Qt::MetaModifier;
    }

    return qtmod;
}

void sendQtKeyEvent(Item* item, bool press, int keyCode, KeyModifier mod)
{
    auto qtitem = dynamic_cast<QtWidgetsItem*>(item);
    if (!qtitem)
        return;

    QWidget* widget = qtitem->qwidget();
    // Send to the focused widget if there is one, otherwise to the widget itself
    QWidget* focusWidget = widget->focusWidget();
    if (!focusWidget) {
        focusWidget = widget;
    }

    auto qtmod = getQtKeyboardModifiers(mod);
    auto keyEvent = new QKeyEvent(press ? QEvent::KeyPress : QEvent::KeyRelease, keyCode, qtmod);
    QApplication::postEvent(focusWidget, keyEvent);
}

} // namespace

void QtWidgetsEvents::mouseDown(Item* item, Point loc, MouseButton button, KeyModifier mod)
{
    QPoint widgetLoc;
    auto widget = getWidgetAndPositionForItem(item, loc, widgetLoc);
    if (!widget)
        return;

    m_pressedMouseButtons |= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

    auto qtmod = getQtKeyboardModifiers(mod);
    // Qt6 requires both local and global positions
    QMouseEvent* event = new QMouseEvent(
        QEvent::MouseButtonPress,
        widgetLoc,
        widget->mapToGlobal(widgetLoc),
        eventCausingButton,
        activeButtons,
        qtmod);
    QApplication::postEvent(widget, event);
}

void QtWidgetsEvents::mouseUp(Item* item, Point loc, MouseButton button, KeyModifier mod)
{
    QPoint widgetLoc;
    auto widget = getWidgetAndPositionForItem(item, loc, widgetLoc);
    if (!widget)
        return;

    // Qt6 expects the mouse to be down during the event
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    m_pressedMouseButtons ^= button;

    auto qtmod = getQtKeyboardModifiers(mod);
    QMouseEvent* event = new QMouseEvent(
        QEvent::MouseButtonRelease,
        widgetLoc,
        widget->mapToGlobal(widgetLoc),
        eventCausingButton,
        activeButtons,
        qtmod);
    QApplication::postEvent(widget, event);
}

void QtWidgetsEvents::mouseMove(Item* item, Point loc)
{
    QPoint widgetLoc;
    auto widget = getWidgetAndPositionForItem(item, loc, widgetLoc);
    if (!widget)
        return;

    Qt::MouseButton activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    widgetLoc.rx() -= 1;
    QMouseEvent* mouseMoveEvent = new QMouseEvent(
        QEvent::MouseMove,
        widgetLoc,
        widget->mapToGlobal(widgetLoc),
        Qt::MouseButton::NoButton,
        activeButtons,
        Qt::NoModifier);
    QApplication::postEvent(widget, mouseMoveEvent);

    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    widgetLoc.rx() += 1;
    mouseMoveEvent = new QMouseEvent(
        QEvent::MouseMove,
        widgetLoc,
        widget->mapToGlobal(widgetLoc),
        Qt::MouseButton::NoButton,
        activeButtons,
        Qt::NoModifier);
    QApplication::postEvent(widget, mouseMoveEvent);
}

void QtWidgetsEvents::stringInput(Item* item, const std::string& text)
{
    auto qtitem = dynamic_cast<QtWidgetsItem*>(item);
    if (!qtitem)
        return;

    QWidget* widget = qtitem->qwidget();
    // Send to the focused widget if there is one, otherwise to the widget itself
    QWidget* focusWidget = widget->focusWidget();
    if (!focusWidget) {
        focusWidget = widget;
    }

    auto keyDownEvent = new QKeyEvent(QEvent::KeyPress, 0 /* key id */, Qt::NoModifier, QString::fromStdString(text));
    QApplication::postEvent(focusWidget, keyDownEvent);
}

void QtWidgetsEvents::keyPress(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, true, keyCode, mod);
}

void QtWidgetsEvents::keyRelease(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, false, keyCode, mod);
}

void QtWidgetsEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content)
{
    auto qtitem = dynamic_cast<QtWidgetsItem*>(item);
    if (!qtitem)
        return;

    QWidget* widget = qtitem->qwidget();

    QPoint widgetLoc(static_cast<int>(loc.x), static_cast<int>(loc.y));

    auto mimeData = new QMimeData();
    QList<QUrl> urlList;
    for (const auto& url : content.urls()) {
        QUrl qurl(QString::fromStdString(url));
        urlList.append(qurl);
    }
    mimeData->setUrls(urlList);

    auto enter = new QDragEnterEvent(widgetLoc, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QApplication::postEvent(widget, enter);

    auto move = new QDragMoveEvent(widgetLoc, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QApplication::postEvent(widget, move);

    auto drop = new QDropEvent(QPointF(widgetLoc), Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QApplication::postEvent(widget, drop);
}

void QtWidgetsEvents::quit()
{
    QApplication::quit();
}

} // namespace spix
