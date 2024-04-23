//
// Created by sebastian on 28.02.24.
//

#pragma once
#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>

namespace spix {

class QtEventFilter : public QObject {
    Q_OBJECT
public:
    QtEventFilter(QObject* parent);

signals:
    void pickerModeEntered(QKeyEvent* event);
    void verifyModeEntered(QKeyEvent* event);
    void pressKey(QKeyEvent* event);
    void modeExited(QKeyEvent* event);
    void pickClick(QMouseEvent* event);
    void verifyClick(QMouseEvent* event);
    void click(QMouseEvent* event);
    void record();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

} // namespace spix