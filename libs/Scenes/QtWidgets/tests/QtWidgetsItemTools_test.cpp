/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <limits>

#include "QtWidgetsTestUtils.h"
#include <gtest/gtest.h>

#include <QtWidgetsItemTools.h>

using Variant = spix::Variant;
using QMLReturnVariant = spix::qt::QMLReturnVariant;

class QtWidgetsItemToolsTest : public WidgetTest {
};

TEST(QtWidgetsItemToolsTestStandalone, NullToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(nullptr));
    EXPECT_TRUE(qvar.isNull());
}

TEST(QtWidgetsItemToolsTestStandalone, BoolToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(true));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::Bool);
    EXPECT_TRUE(qvar.toBool());
}

TEST(QtWidgetsItemToolsTestStandalone, NumberToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(-137LL));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::LongLong);
    EXPECT_EQ(qvar.toLongLong(), -137);
}

TEST(QtWidgetsItemToolsTestStandalone, LongNumberToQVariant)
{
    auto number = std::numeric_limits<long long>::max();
    auto qvar = spix::qt::VariantToQVariant(Variant(number));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::LongLong);
    EXPECT_EQ(qvar.toLongLong(), number);
}

TEST(QtWidgetsItemToolsTestStandalone, FloatNumberToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(2.1));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::Double);
    EXPECT_EQ(qvar.toDouble(), 2.1);
}

TEST(QtWidgetsItemToolsTestStandalone, StringToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(std::string("hello strings!")));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::QString);
    EXPECT_EQ(qvar.toString(), "hello strings!");
}

TEST(QtWidgetsItemToolsTestStandalone, UnicodeToQVariant)
{
    auto qvar = spix::qt::VariantToQVariant(Variant(std::string(u8"☀☁☂☃☄★☆☇")));
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::QString);
    EXPECT_EQ(qvar.toString(), u8"☀☁☂☃☄★☆☇");
}

TEST(QtWidgetsItemToolsTestStandalone, DateToQVariant)
{
    auto var = Variant(std::chrono::system_clock::from_time_t(1651775070));
    auto qvar = spix::qt::VariantToQVariant(var);
    EXPECT_EQ(qvar.typeId(), QMetaType::Type::QDateTime);
    EXPECT_EQ(qvar.toDateTime(), QDateTime::fromSecsSinceEpoch(1651775070));
}

TEST(QtWidgetsItemToolsTestStandalone, ListToQVariant)
{
    auto list = Variant::ListType {Variant(true), Variant(137LL), Variant(std::string("hello strings!"))};
    auto var = Variant(list);
    auto qvar = spix::qt::VariantToQVariant(var);

    EXPECT_EQ(qvar.typeId(), QMetaType::Type::QVariantList);
    auto qlist = qvar.toList();
    EXPECT_EQ(qlist.size(), 3);
    EXPECT_EQ(qlist[0].typeId(), QMetaType::Type::Bool);
    EXPECT_TRUE(qlist[0].toBool());
    EXPECT_EQ(qlist[1].typeId(), QMetaType::Type::LongLong);
    EXPECT_EQ(qlist[1].toLongLong(), 137);
    EXPECT_EQ(qlist[2].typeId(), QMetaType::Type::QString);
    EXPECT_EQ(qlist[2].toString(), "hello strings!");
}

TEST(QtWidgetsItemToolsTestStandalone, MapToQVariant)
{
    auto map = Variant::MapType {
        {"key0", Variant(true)}, {"key1", Variant(137LL)}, {"key2", Variant(std::string("hello strings!"))}};
    auto var = Variant(map);
    auto qvar = spix::qt::VariantToQVariant(var);

    EXPECT_EQ(qvar.typeId(), QMetaType::Type::QVariantMap);
    auto qmap = qvar.toMap();
    EXPECT_EQ(qmap.size(), 3);
    EXPECT_EQ(qmap["key0"].typeId(), QMetaType::Type::Bool);
    EXPECT_TRUE(qmap["key0"].toBool());
    EXPECT_EQ(qmap["key1"].typeId(), QMetaType::Type::LongLong);
    EXPECT_EQ(qmap["key1"].toLongLong(), 137);
    EXPECT_EQ(qmap["key2"].typeId(), QMetaType::Type::QString);
    EXPECT_EQ(qmap["key2"].toString(), "hello strings!");
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnNullToVariant)
{
    auto retvar = QMLReturnVariant(nullptr);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(nullptr));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnBoolToVariant)
{
    auto retvar = QMLReturnVariant(true);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(true));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnIntToVariant)
{
    auto retvar = QMLReturnVariant(137);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(137LL));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnFloatToVariant)
{
    auto retvar = QMLReturnVariant(137.0f);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_FLOAT_EQ(std::get<double>(var), 137.0);
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnDoubleToVariant)
{
    auto retvar = QMLReturnVariant(137.0);
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(137.0));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnStringToVariant)
{
    auto retvar = QMLReturnVariant(QString("hello strings!"));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::string("hello strings!")));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnUnicodeToVariant)
{
    auto retvar = QMLReturnVariant(QString(u8"☀☁☂☃☄★☆☇"));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::string(u8"☀☁☂☃☄★☆☇")));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnDateToVariant)
{
    auto retvar = QMLReturnVariant(QDateTime::fromSecsSinceEpoch(1651775070));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(std::chrono::system_clock::from_time_t(1651775070)));
}

TEST(QtWidgetsItemToolsTestStandalone, ReturnQVariantToVariant)
{
    auto retvar = QMLReturnVariant(QVariant(true));
    auto var = spix::qt::QMLReturnVariantToVariant(retvar);

    EXPECT_EQ(var, Variant(true));
}

TEST_F(QtWidgetsItemToolsTest, TypeStringForWidgetStripsQPrefix)
{
    auto widget = CreateTestWidget();
    QString typeName = spix::qt::TypeStringForWidget(widget);
    EXPECT_EQ(typeName, "Widget");
    delete widget;
}

TEST_F(QtWidgetsItemToolsTest, TypeStringForPushButton)
{
    auto button = new QPushButton("Test");
    QString typeName = spix::qt::TypeStringForWidget(button);
    EXPECT_EQ(typeName, "PushButton");
    delete button;
}

TEST_F(QtWidgetsItemToolsTest, TypeStringForLabel)
{
    auto label = new QLabel("Test");
    QString typeName = spix::qt::TypeStringForWidget(label);
    EXPECT_EQ(typeName, "Label");
    delete label;
}

TEST_F(QtWidgetsItemToolsTest, GetObjectNameReturnsObjectName)
{
    auto widget = CreateTestWidget("myTestWidget");
    QString name = spix::qt::GetObjectName(widget);
    EXPECT_EQ(name, "myTestWidget");
    delete widget;
}

TEST_F(QtWidgetsItemToolsTest, GetObjectNameReturnsEmptyForNull)
{
    QString name = spix::qt::GetObjectName(nullptr);
    EXPECT_EQ(name, "");
}

TEST_F(QtWidgetsItemToolsTest, ForEachChildIteratesChildWidgets)
{
    auto widget = new QWidget();
    auto layout = new QVBoxLayout(widget);

    auto button1 = new QPushButton("Button 1", widget);
    button1->setObjectName("button1");
    layout->addWidget(button1);

    auto button2 = new QPushButton("Button 2", widget);
    button2->setObjectName("button2");
    layout->addWidget(button2);

    std::vector<QString> childNames;
    spix::qt::ForEachChild(widget, [&childNames](QObject* child) -> bool {
        childNames.push_back(child->objectName());
        return true;
    });

    // Should find both buttons
    EXPECT_EQ(childNames.size(), 2);
    EXPECT_TRUE(std::find(childNames.begin(), childNames.end(), "button1") != childNames.end());
    EXPECT_TRUE(std::find(childNames.begin(), childNames.end(), "button2") != childNames.end());

    delete widget;
}

TEST_F(QtWidgetsItemToolsTest, ForEachChildStopsOnFalse)
{
    auto widget = new QWidget();
    auto layout = new QVBoxLayout(widget);

    auto button1 = new QPushButton("Button 1", widget);
    button1->setObjectName("button1");
    layout->addWidget(button1);

    auto button2 = new QPushButton("Button 2", widget);
    button2->setObjectName("button2");
    layout->addWidget(button2);

    int count = 0;
    spix::qt::ForEachChild(widget, [&count](QObject*) -> bool {
        count++;
        return false; // Stop after first child
    });

    EXPECT_EQ(count, 1);

    delete widget;
}
