/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <limits>

#include "QtTestUtils.h"
#include <gtest/gtest.h>

#include <QtItemTools.h>

using Variant = spix::Variant;
using QMLReturnVariant = spix::qt::QMLReturnVariant;

class QtItemToolsTestWithQMLEngine : public QMLEngineTest {
};

TEST(QtItemToolsTest, NullToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(nullptr));
    EXPECT_TRUE(qvar.isNull());
}

TEST(QtItemToolsTest, BoolToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(true));
    EXPECT_EQ(qvar.type(), QMetaType::Type::Bool);
    EXPECT_TRUE(qvar.toBool());
}

TEST(QtItemToolsTest, NumberToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(-137LL));
    EXPECT_EQ(qvar.type(), QMetaType::Type::LongLong);
    EXPECT_EQ(qvar.toLongLong(), -137);
}

TEST(QtItemToolsTest, LongNumberToQVariant)
{
    auto number = std::numeric_limits<long long>::max();
    auto qvar = spix::qt::VariantToQVariant(Variant(number));
    EXPECT_EQ(qvar.type(), QMetaType::Type::LongLong);
    EXPECT_EQ(qvar.toLongLong(), number);
}

TEST(QtItemToolsTest, FloatNumberToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(2.1));
    EXPECT_EQ(qvar.type(), QMetaType::Type::Double);
    EXPECT_EQ(qvar.toDouble(), 2.1);
}

TEST(QtItemToolsTest, StringToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(std::string("hello strings!")));
    EXPECT_EQ(qvar.type(), QMetaType::Type::QString);
    EXPECT_EQ(qvar.toString(), "hello strings!");
}

TEST(QtItemToolsTest, UnicodeToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(std::string(u8"☀☁☂☃☄★☆☇")));
    EXPECT_EQ(qvar.type(), QMetaType::Type::QString);
    EXPECT_EQ(qvar.toString(), u8"☀☁☂☃☄★☆☇");
}

TEST(QtItemToolsTest, DateToQVariant)
{
    auto var = Variant(std::chrono::system_clock::from_time_t(1651775070));
    auto qvar = spix::qt::VariantToQVariant(var);
    EXPECT_EQ(qvar.type(), QMetaType::Type::QDateTime);
    EXPECT_EQ(qvar.toDateTime(), QDateTime::fromSecsSinceEpoch(1651775070));
}

TEST(QtItemToolsTest, ListToQVariant)
{
    auto list = Variant::ListType {Variant(true), Variant(137LL), Variant(std::string("hello strings!"))};
    auto var = Variant(list);
    auto qvar = spix::qt::VariantToQVariant(var);

    EXPECT_EQ(qvar.type(), QMetaType::Type::QVariantList);
    auto qlist = qvar.toList();
    EXPECT_EQ(qlist.size(), 3);
    EXPECT_EQ(qlist[0].type(), QMetaType::Type::Bool);
    EXPECT_TRUE(qlist[0].toBool());
    EXPECT_EQ(qlist[1].type(), QMetaType::Type::LongLong);
    EXPECT_EQ(qlist[1].toLongLong(), 137);
    EXPECT_EQ(qlist[2].type(), QMetaType::Type::QString);
    EXPECT_EQ(qlist[2].toString(), "hello strings!");
}

TEST(QtItemToolsTest, MapToQVariant)
{
    auto map = Variant::MapType {
        {"key0", Variant(true)}, {"key1", Variant(137LL)}, {"key2", Variant(std::string("hello strings!"))}};
    auto var = Variant(map);
    auto qvar = spix::qt::VariantToQVariant(var);

    EXPECT_EQ(qvar.type(), QMetaType::Type::QVariantMap);
    auto qmap = qvar.toMap();
    EXPECT_EQ(qmap.size(), 3);
    EXPECT_EQ(qmap["key0"].type(), QMetaType::Type::Bool);
    EXPECT_TRUE(qmap["key0"].toBool());
    EXPECT_EQ(qmap["key1"].type(), QMetaType::Type::LongLong);
    EXPECT_EQ(qmap["key1"].toLongLong(), 137);
    EXPECT_EQ(qmap["key2"].type(), QMetaType::Type::QString);
    EXPECT_EQ(qmap["key2"].toString(), "hello strings!");
}

TEST(QtItemToolsTest, MapOfListToQVariant)
{
    auto map = Variant::MapType {
        {"key0", Variant::ListType {Variant(true), Variant(137LL), Variant(std::string("hello strings!"))}}};
    auto var = Variant(map);
    auto qvar = spix::qt::VariantToQVariant(var);

    EXPECT_EQ(qvar.type(), QMetaType::Type::QVariantMap);
    auto qmap = qvar.toMap();
    EXPECT_EQ(qmap.size(), 1);
    auto qinner = qmap["key0"];
    EXPECT_EQ(qinner.type(), QMetaType::Type::QVariantList);
    auto qinnerlist = qinner.toList();
    EXPECT_EQ(qinnerlist.size(), 3);
    EXPECT_EQ(qinnerlist[0].type(), QMetaType::Type::Bool);
    EXPECT_TRUE(qinnerlist[0].toBool());
    EXPECT_EQ(qinnerlist[1].type(), QMetaType::Type::LongLong);
    EXPECT_EQ(qinnerlist[1].toLongLong(), 137);
    EXPECT_EQ(qinnerlist[2].type(), QMetaType::Type::QString);
    EXPECT_EQ(qinnerlist[2].toString(), "hello strings!");
}

TEST(QtItemToolsTest, ReturnNullToVariant)
{
    auto retvar = QMLReturnVariant(nullptr);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(nullptr));
}

TEST(QtItemToolsTest, ReturnBoolToVariant)
{
    auto retvar = QMLReturnVariant(true);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(true));
}

TEST(QtItemToolsTest, ReturnIntToVariant)
{
    auto retvar = QMLReturnVariant(137);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(137LL));
}

TEST(QtItemToolsTest, ReturnFloatToVariant)
{
    auto retvar = QMLReturnVariant(137.0f);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_FLOAT_EQ(std::get<double>(var), 137.0);
}

TEST(QtItemToolsTest, ReturnDoubleToVariant)
{
    auto retvar = QMLReturnVariant(137.0);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(137.0));
}

TEST(QtItemToolsTest, ReturnStringToVariant)
{
    auto retvar = QMLReturnVariant(QString("hello strings!"));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::string("hello strings!")));
}

TEST(QtItemToolsTest, ReturnUnicodeToVariant)
{
    auto retvar = QMLReturnVariant(QString(u8"☀☁☂☃☄★☆☇"));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::string(u8"☀☁☂☃☄★☆☇")));
}

TEST(QtItemToolsTest, ReturnDateToVariant)
{
    auto retvar = QMLReturnVariant(QDateTime::fromSecsSinceEpoch(1651775070));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::chrono::system_clock::from_time_t(1651775070)));
}

TEST(QtItemToolsTest, ReturnQVariantToVariant)
{
    auto retvar = QMLReturnVariant(QVariant(true));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(true));
}

TEST_F(QtItemToolsTestWithQMLEngine, TestFixture)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return 147; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    EXPECT_EQ(ret.type(), QMetaType::Type::Int);
    EXPECT_EQ(ret.toInt(), 147);
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLNullToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return null; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(nullptr));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLUndefinedToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return undefined; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(nullptr));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLNoneToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(nullptr));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLBoolToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return true; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(true));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLIntToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return 137; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(137LL));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLLongIntToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return 9223372036854775807; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    // long numbers are upcast to doubles in QML
    EXPECT_EQ(val, Variant(9223372036854775807.0));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLFloatToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return 14.1; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(14.1));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLStringToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return 'hello strings!'; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(std::string("hello strings!")));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLUnicodeToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod(u8"function test() { return '☀☁☂☃☄★☆☇'; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(std::string(u8"☀☁☂☃☄★☆☇")));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLDateToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return new Date(1651775070*1000); }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);
    EXPECT_EQ(val, Variant(std::chrono::system_clock::from_time_t(1651775070)));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLListToVariant)
{
    QQuickItem* element
        = this->GetQQuickItemWithMethod("function test() { return [true, 137, undefined, 'hello strings!']; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);

    auto expect
        = Variant::ListType {Variant(true), Variant(137LL), Variant(nullptr), Variant(std::string("hello strings!"))};
    EXPECT_EQ(val, Variant(expect));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLMapToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod(
        "function test() { return {'key0': true, 'key1': 137, 'key2': null, 'key3': 'hello strings!'}; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);

    auto expect = Variant::MapType {{"key0", Variant(true)}, {"key1", Variant(137LL)}, {"key2", Variant(nullptr)},
        {"key3", Variant(std::string("hello strings!"))}};
    EXPECT_EQ(val, Variant(expect));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLMapWeirdKeysToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod(
        "function test() { return {1234: true, false: true, null: true, undefined: true}; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);

    auto expect = Variant::MapType {
        {"1234", Variant(true)}, {"false", Variant(true)}, {"null", Variant(true)}, {"undefined", Variant(true)}};
    EXPECT_EQ(val, Variant(expect));
}

TEST_F(QtItemToolsTestWithQMLEngine, QMLMapOfListToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod(
        "function test() { return {'key0': [true, 137, undefined, 'hello strings!']}; }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    Variant val = spix::qt::QVariantToVariant(ret);

    auto innerListExpect
        = Variant::ListType {Variant(true), Variant(137LL), Variant(nullptr), Variant(std::string("hello strings!"))};
    auto expect = Variant::MapType {{"key0", Variant(innerListExpect)}};
    EXPECT_EQ(val, Variant(expect));
}

TEST_F(QtItemToolsTestWithQMLEngine, UnknownPrimitiveToVariant)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { return Qt.point(0, 20); }");
    QVariant ret;
    EXPECT_TRUE(QMetaObject::invokeMethod(element, "test", Q_RETURN_ARG(QVariant, ret)));
    EXPECT_THROW(spix::qt::QVariantToVariant(ret), std::runtime_error);
}

TEST_F(QtItemToolsTestWithQMLEngine, GetMethodFinds)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test() { }");
    QMetaMethod ret;
    EXPECT_TRUE(spix::qt::GetMethodMetaForArgs(*element, "test", {}, ret));
    EXPECT_EQ(ret.methodSignature(), "test()");
}

TEST_F(QtItemToolsTestWithQMLEngine, GetMethodFindsWithArg)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("function test(arg) { }");
    QMetaMethod ret;
    EXPECT_TRUE(spix::qt::GetMethodMetaForArgs(*element, "test", {QVariant(true)}, ret));
    EXPECT_EQ(ret.methodSignature(), "test(QVariant)");
}

TEST_F(QtItemToolsTestWithQMLEngine, GetMethodFindsWithTypedArg)
{
    // Qt5 doesn't support javascript type annotations, but we can skirt this by using a signal
    QQuickItem* element = this->GetQQuickItemWithMethod("signal test(bool arg)");
    QMetaMethod ret;
    EXPECT_TRUE(spix::qt::GetMethodMetaForArgs(*element, "test", {QVariant(true)}, ret));
    EXPECT_EQ(ret.methodSignature(), "test(bool)");
}

TEST_F(QtItemToolsTestWithQMLEngine, GetMethodFindsWithTypedConvertableArg)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("signal test(bool arg)");
    QMetaMethod ret;
    EXPECT_TRUE(spix::qt::GetMethodMetaForArgs(*element, "test", {QVariant(1)}, ret));
    EXPECT_EQ(ret.methodSignature(), "test(bool)");
}

TEST_F(QtItemToolsTestWithQMLEngine, GetMethodFalseIfNonConvertable)
{
    QQuickItem* element = this->GetQQuickItemWithMethod("signal test(bool arg)");
    QMetaMethod ret;
    EXPECT_FALSE(spix::qt::GetMethodMetaForArgs(*element, "test", {QVariant(QDateTime())}, ret));
}
