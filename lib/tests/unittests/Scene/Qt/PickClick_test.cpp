#include "Spix/QtQmlBot.h"
#include <gtest/gtest.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTest>
#include <QQmlComponent>
#include <QQuickWindow>

class PickClickTest : public ::testing::Test {
protected:
    PickClickTest()
    : fakeArg {"hello"}
    , fakeArgPtr(fakeArg)
    , fakeArgList(&fakeArgPtr)
    , fakeArgCount(1)
    , app(fakeArgCount, fakeArgList)
    , engine()
    , component(&engine)
    , w()
    {
        w.show();
        w.setObjectName("mainWindow");
    }


    char fakeArg[50];
    char* fakeArgPtr;
    char** fakeArgList;
    int fakeArgCount;
    QGuiApplication app;
    QQmlApplicationEngine engine;
    QQmlComponent component;
    QQuickWindow w;
};


TEST_F(PickClickTest, CursorShape){
    auto bot = spix::QtQmlBot();
    QTest::qWait(10);

    QTest::keyPress(&w, Qt::Key_Control, Qt::KeyboardModifier::ShiftModifier);

    EXPECT_TRUE(QGuiApplication::overrideCursor() != nullptr);
    EXPECT_TRUE(QGuiApplication::overrideCursor()->shape() == Qt::CursorShape::CrossCursor);

    QTest::keyRelease(&w, Qt::Key_Shift);
    QTest::keyRelease(&w, Qt::Key_Control);
}


TEST_F(PickClickTest, ClickItem){
    auto bot = spix::QtQmlBot();
    auto l = QTest::qWaitForWindowExposed(&w);
    w.resize(10, 10);

    testing::internal::CaptureStderr();

    QTest::mouseClick(&w, Qt::MouseButton::LeftButton, Qt::KeyboardModifier::ShiftModifier | Qt::KeyboardModifier::ControlModifier, QPoint(1, 1));

    std::string out = testing::internal::GetCapturedStderr();

    EXPECT_EQ(out, "Path:  'mainWindow'\n");
}
