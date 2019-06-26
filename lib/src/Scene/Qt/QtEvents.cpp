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

Qt::MouseButton getQtMouseButtonValue(Events::MouseButton button)
{
    unsigned qtButton = Qt::MouseButton::NoButton;
    
    if(button & Events::MouseButtons::left){
        qtButton |= Qt::MouseButton::LeftButton;
    }
    
    if(button & Events::MouseButtons::right){
        qtButton |= Qt::MouseButton::RightButton;
    }
    
    if(button & Events::MouseButtons::middle){
        qtButton |= Qt::MouseButton::MiddleButton;
    }
    
    return static_cast<Qt::MouseButton>(qtButton);
}

}



void QtEvents::mouseDown(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if(!window) return;
    
    m_pressedMouseButtons |= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    
    QMouseEvent* event = new QMouseEvent(
        QEvent::MouseButtonPress, windowLoc, eventCausingButton, activeButtons, 0);
    QGuiApplication::postEvent(window, event);
}

void QtEvents::mouseUp(Item* item, Point loc, MouseButton button)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if(!window) return;
    
    m_pressedMouseButtons ^= button;
    Qt::MouseButton eventCausingButton = getQtMouseButtonValue(button);
    Qt::MouseButtons activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    
    QMouseEvent* event = new QMouseEvent(
        QEvent::MouseButtonRelease, windowLoc, eventCausingButton, activeButtons, 0);
    QGuiApplication::postEvent(window, event);
}

void QtEvents::mouseMove(Item* item, Point loc)
{
    QPointF windowLoc;
    auto window = getWindowAndPositionForItem(item, loc, windowLoc);
    if(!window) return;
    
    Qt::MouseButton activeButtons = getQtMouseButtonValue(m_pressedMouseButtons);
    
    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() -= 1;
    QMouseEvent* mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, 0);
    QGuiApplication::postEvent(window, mouseMoveEvent);

    // Wiggle the cursor a bit. This is needed to correctly recognize drag events
    windowLoc.rx() += 1;
    mouseMoveEvent
        = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::NoButton, activeButtons, 0);
    QGuiApplication::postEvent(window, mouseMoveEvent);
}


void QtEvents::stringInput(Item* item, const std::string& text)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    auto keyDownEvent
        = new QKeyEvent(QEvent::KeyPress, 0 /* key id */, 0 /* modifiers */, QString::fromStdString(text));
    QGuiApplication::postEvent(window, keyDownEvent);
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

    auto enter = new QDragEnterEvent(
        windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData, Qt::MouseButton::NoButton, 0);
    QGuiApplication::postEvent(window, enter);

    auto move = new QDragMoveEvent(
        windowLocInt, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData, Qt::MouseButton::NoButton, 0);
    QGuiApplication::postEvent(window, move);

    auto drop = new QDropEvent(
        windowLoc, Qt::CopyAction | Qt::MoveAction | Qt::LinkAction, mimeData, Qt::MouseButton::NoButton, 0);
    QGuiApplication::postEvent(window, drop);
}

void QtEvents::quit()
{
    QGuiApplication::quit();
}

} // namespace uibot
