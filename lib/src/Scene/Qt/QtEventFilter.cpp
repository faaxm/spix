//
// Created by sebastian on 28.02.24.
//

#include <QEvent>
#include <QObject>

#include "QtEventFilter.h"
#include <QDebug>
namespace spix {

QtEventFilter::QtEventFilter(QObject* parent)
: QObject(parent)
{
}

bool QtEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ShiftModifier | Qt::ControlModifier)) {
            emit pickerModeEntered(keyEvent);
            return false;
        } else if (keyEvent->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::AltModifier)) {
            emit verifyModeEntered(keyEvent);
            return false;
        } else if (keyEvent->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ControlModifier)
            && keyEvent->key() == Qt::Key_R) {
            emit record();
            return false;
        } else {
            emit pressKey(keyEvent);
            return false;
        }

    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == 16777249 || keyEvent->key() == 16777248 || keyEvent->key() == 16777251) {
            emit modeExited(keyEvent);
            return false;
        }

    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseClick = static_cast<QMouseEvent*>(event);
        if (mouseClick->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ShiftModifier | Qt::ControlModifier)) {
            emit pickClick(mouseClick);
            return true;
        } else if (mouseClick->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::AltModifier)) {
            emit verifyClick(mouseClick);
            return true;
        } else {
            emit click(mouseClick);
            return false;
        }
    }

    return QObject::eventFilter(obj, event);
}

} // namespace spix