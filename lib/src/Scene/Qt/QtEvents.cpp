/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtEvents.h"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>

#include <Spix/Data/PasteboardContent.h>

#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

// comment this to use mouse events on window level
// if uncommented, the mouse events are triggered on the given item
// #define WINDOW_MODE

namespace spix {

namespace {

QQuickWindow* getWindowAndPositionForItem(Item* item, Point relToItemPos, QPointF& windowPos)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return nullptr;
    auto window = qtitem->qquickitem()->window();

    QPointF qtlocalPoint(relToItemPos.x, relToItemPos.y);
    windowPos = qtitem->qquickitem()->mapToScene(qtlocalPoint);

    return window;
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
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto qtmod = getQtKeyboardModifiers(mod);
    auto keyEvent = new QKeyEvent(press ? QEvent::KeyPress : QEvent::KeyRelease, keyCode, qtmod);

#ifdef WINDOW_MODE
    auto window = qtitem->qquickitem()->window();
    QGuiApplication::postEvent(window, keyEvent);
#else
    QGuiApplication::postEvent(qtitem->qquickitem(), keyEvent);
#endif
}

} // namespace

void QtEvents::mouseDown(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    m_pressedMouseButtons |= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

    QMouseEvent* event = new QMouseEvent(
        QEvent::MouseButtonPress, windowLoc, eventCausingButton, activeButtons, Qt::NoModifier);

#ifdef WINDOW_MODE
    QGuiApplication::postEvent(window, event);
#else
    QGuiApplication::postEvent(qtitem->qquickitem(), event);
#endif

}

void QtEvents::mouseUp(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
    // Qt6 expects the mouse to be down during the event
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    m_pressedMouseButtons ^= button;
#else
    // Qt5 expects the mouse to be up during the event
    m_pressedMouseButtons ^= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
#endif

    QMouseEvent* event = new QMouseEvent(QEvent::MouseButtonRelease, windowLoc,
        eventCausingButton, activeButtons, Qt::NoModifier);

#ifdef WINDOW_MODE
    QGuiApplication::postEvent(window, event);
#else
    QGuiApplication::postEvent(qtitem->qquickitem(), event);
#endif
}

void QtEvents::mouseMove(Item* item, Point loc)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if (!window)
        return;

    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    Qt::MouseButton activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);

           // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() -= 1;
    QMouseEvent* mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, Qt::NoModifier);
    QGuiApplication::postEvent(qtitem->qquickitem(), mouseMoveEvent);

           // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() += 1;
    mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, Qt::NoModifier);

#ifdef WINDOW_MODE
    QGuiApplication::postEvent(window, mouseMoveEvent);
#else
    QGuiApplication::postEvent(qtitem->qquickitem(), mouseMoveEvent);
#endif
}

void QtEvents::stringInput(Item* item, const std::string& text)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    auto keyDownEvent = new QKeyEvent(QEvent::KeyPress, 0 /* key id */, Qt::NoModifier, QString::fromStdString(text));
    QGuiApplication::postEvent(window, keyDownEvent);
}

void QtEvents::keyPress(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, true, keyCode, mod);
}

void QtEvents::keyRelease(Item* item, int keyCode, KeyModifier mod)
{
    sendQtKeyEvent(item, false, keyCode, mod);
}

void QtEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    QPointF qtlocalPoint(loc.x, loc.y);
    QPointF windowLoc = qtitem->qquickitem()->mapToScene(qtlocalPoint);
    QPoint windowLocInt(windowLoc.rx(), windowLoc.ry());

    auto mimeData = new QMimeData();
    QList<QUrl> urlList;
    for (const auto& url : content.urls()) {
        QUrl qurl(QString::fromStdString(url));
        urlList.append(qurl);
    }
    mimeData->setUrls(urlList);

    auto enter = new QDragEnterEvent(windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, enter);

    auto move = new QDragMoveEvent(windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, move);

    auto drop = new QDropEvent(windowLoc, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData,
        Qt::MouseButton::NoButton, Qt::NoModifier);
    QGuiApplication::postEvent(window, drop);
}

void QtEvents::quit()
{
    QGuiApplication::quit();
}

} // namespace spix
