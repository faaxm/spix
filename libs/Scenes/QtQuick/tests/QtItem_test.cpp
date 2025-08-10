/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <limits>

#include <gtest/gtest.h>

#include <QtItem.h>
#include <QtItemTools.h>

#include "QtTestUtils.h"

using Variant = spix::Variant;
using QMLReturnVariant = spix::qt::QMLReturnVariant;

class QtItemTestWithQMLEngine : public QMLEngineTest {
};

template <class... Ts>
QList<QList<QVariant>> MakeQSpyCall(Ts... args)
{
    return QList(std::initializer_list<QList<QVariant>>({QList {QVariant(args)...}}));
}

TEST_F(QtItemTestWithQMLEngine, TestFixture)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(int arg)", SIGNAL(test(int)));
    EXPECT_TRUE(QMetaObject::invokeMethod(obj, "test", Q_ARG(int, 3)));
    EXPECT_EQ(spy->count(), 1);
    EXPECT_EQ((*spy)[0].count(), 1);
    EXPECT_EQ((*spy)[0][0].type(), QMetaType::Int);
    EXPECT_EQ((*spy)[0][0].toInt(), 3);
}

TEST_F(QtItemTestWithQMLEngine, InvokesIntArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(int arg)", SIGNAL(test(int)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(3LL)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(3));
}

TEST_F(QtItemTestWithQMLEngine, InvokesBoolArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(bool arg)", SIGNAL(test(bool)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(true)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(true));
}

TEST_F(QtItemTestWithQMLEngine, InvokesDoubleArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(double arg)", SIGNAL(test(double)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(2.7)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(2.7));
}

TEST_F(QtItemTestWithQMLEngine, InvokesRealArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(real arg)", SIGNAL(test(double)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(2.7)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(2.7));
}

TEST_F(QtItemTestWithQMLEngine, InvokesStringArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(string arg)", SIGNAL(test(QString)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(std::string("hello strings!"))}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall("hello strings!"));
}

TEST_F(QtItemTestWithQMLEngine, InvokesDateArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(date arg)", SIGNAL(test(QDateTime)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(std::chrono::system_clock::from_time_t(1651775070))}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(QDateTime::fromSecsSinceEpoch(1651775070)));
}

TEST_F(QtItemTestWithQMLEngine, InvokesVarArg)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(var arg)", SIGNAL(test(QVariant)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(true)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(true));
}

TEST_F(QtItemTestWithQMLEngine, InvokesVarArgWithList)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(var arg)", SIGNAL(test(QVariant)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(Variant::ListType {Variant(true), Variant(false)})}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(QVariantList {QVariant(true), QVariant(false)}));
}

TEST_F(QtItemTestWithQMLEngine, InvokesVarArgWithMap)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(var arg)", SIGNAL(test(QVariant)));
    spix::QtItem item(obj);
    Variant ret;
    auto arg = Variant(Variant::MapType {{"key0", Variant(true)}, {"key1", Variant(false)}});
    EXPECT_TRUE(item.invokeMethod("test", {arg}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(QVariantMap {{"key0", QVariant(true)}, {"key1", QVariant(false)}}));
}

TEST_F(QtItemTestWithQMLEngine, InvokesVarArgWithNull)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(var arg)", SIGNAL(test(QVariant)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(nullptr)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall(QVariant()));
}

TEST_F(QtItemTestWithQMLEngine, InvokeHandlesNoMatch)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(point arg)", SIGNAL(test(QPointF)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_FALSE(item.invokeMethod("test", {Variant(true)}, ret));
}

TEST_F(QtItemTestWithQMLEngine, InvokeHandlesConversion)
{
    auto [obj, spy] = this->GetQQuickItemWithSignalSpy("signal test(string arg)", SIGNAL(test(QString)));
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {Variant(true)}, ret));
    EXPECT_EQ(*spy, MakeQSpyCall("true"));
}

// QML Return type can only be specified on Qt >= 6
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

TEST_F(QtItemTestWithQMLEngine, InvokeHandlesReturnType)
{
    auto obj = this->GetQQuickItemWithMethod("function test(): int { return 5; }");
    spix::QtItem item(obj);
    Variant ret;
    EXPECT_TRUE(item.invokeMethod("test", {}, ret));
    EXPECT_EQ(ret, Variant(5LL));
}

#endif // Qt >= 6.0.0
