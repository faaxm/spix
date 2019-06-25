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

namespace uibot {

void QtEvents::mouseClick(Item* item, Point loc, bool press, bool release)
{
    auto qtitem = dynamic_cast<QtItem*>(item);
    if (!qtitem)
        return;

    auto window = qtitem->qquickitem()->window();

    QPointF qtlocalPoint(loc.x, loc.y);
    QPointF windowLoc = qtitem->qquickitem()->mapToScene(qtlocalPoint);

    if (press) {
        QMouseEvent* mousePressEvent = new QMouseEvent(
            QEvent::MouseButtonPress, windowLoc, Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, 0);
        QGuiApplication::postEvent(window, mousePressEvent);
    }

    if (release) {
        QMouseEvent* mouseReleaseEvent = new QMouseEvent(
            QEvent::MouseButtonRelease, windowLoc, Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, 0);
        QGuiApplication::postEvent(window, mouseReleaseEvent);
    }

    if (!press && !release) {
        QMouseEvent* mouseMoveEvent
            = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, 0);
        QGuiApplication::postEvent(window, mouseMoveEvent);

        // Wiggle the cursor a bit. This is needed to correctly recognize drag events
        windowLoc.rx() += 1;
        mouseMoveEvent
            = new QMouseEvent(QEvent::MouseMove, windowLoc, Qt::MouseButton::LeftButton, Qt::MouseButton::NoButton, 0);
        QGuiApplication::postEvent(window, mouseMoveEvent);
    }
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
