#include "QtEventFilter.h"
#include <QDebug>
#include <QEvent>



namespace spix{

QtEventFilter::QtEventFilter(QObject* parent) : QObject(parent) {}

bool QtEventFilter::eventFilter(QObject* obj, QEvent* e){
    if(e->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);

        if(keyEvent->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ShiftModifier | Qt::ControlModifier)) {
            emit pickerModeEntered(keyEvent);
            return false;
        }
    } else if(e->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);

        if(keyEvent->key() == Qt::Key::Key_Shift || keyEvent->key() == Qt::Key::Key_Control){
            emit pickerModeExited(keyEvent);
            return false;
        }
    } else if(e->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseClick = static_cast<QMouseEvent*>(e);
        if(mouseClick->modifiers() == QFlags<Qt::KeyboardModifier>(Qt::ShiftModifier | Qt::ControlModifier)) {
            emit pickClick(mouseClick);
            return true;
        }
        return false;
    }

    return QObject::eventFilter(obj, e);
}

} // namespace spix

