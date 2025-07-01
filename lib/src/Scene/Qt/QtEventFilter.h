#pragma once

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>



namespace spix {

class QtEventFilter : public QObject {

    Q_OBJECT

public:
    QtEventFilter(QObject* parent);

signals:
    void pickerModeEntered(QKeyEvent* e);
    void pickerModeExited(QKeyEvent* e);
    void pickClick(QMouseEvent* e);

protected:
    bool eventFilter(QObject* obj, QEvent* e) override;

};

} // namespace spix
