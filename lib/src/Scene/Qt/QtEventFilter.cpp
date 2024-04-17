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
        }
    } else if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == 16777249 || keyEvent->key() == 16777248) {
            emit pickerModeExited(keyEvent);
            return false;
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseClick = static_cast<QMouseEvent*>(event);
        if (mouseClick->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ShiftModifier | Qt::ControlModifier)) {
            emit pickClick(mouseClick);
            return true;
        }
        return false;
    }

    return QObject::eventFilter(obj, event);
}

} // namespace spix