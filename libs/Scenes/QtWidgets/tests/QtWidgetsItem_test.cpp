/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <limits>

#include <gtest/gtest.h>

#include <QtWidgetsItem.h>
#include <QtWidgetsItemTools.h>

#include "QtWidgetsTestUtils.h"

using Variant = spix::Variant;

class QtWidgetsItemTest : public WidgetTest {
};

TEST_F(QtWidgetsItemTest, SizeReturnsWidgetSize)
{
    auto widget = CreateTestWidget();
    widget->resize(300, 150);

    spix::QtWidgetsItem item(widget);
    auto size = item.size();

    EXPECT_EQ(size.width, 300.0);
    EXPECT_EQ(size.height, 150.0);

    delete widget;
}

TEST_F(QtWidgetsItemTest, PositionReturnsGlobalPosition)
{
    auto widget = CreateTestWidget();
    widget->move(100, 50);
    widget->show();

    spix::QtWidgetsItem item(widget);
    auto pos = item.position();

    // Position should be global coordinates
    auto expectedPos = widget->mapToGlobal(QPoint(0, 0));
    EXPECT_EQ(pos.x, static_cast<double>(expectedPos.x()));
    EXPECT_EQ(pos.y, static_cast<double>(expectedPos.y()));

    delete widget;
}

TEST_F(QtWidgetsItemTest, BoundsReturnsCombinedPositionAndSize)
{
    auto widget = CreateTestWidget();
    widget->resize(200, 100);
    widget->move(50, 25);
    widget->show();

    spix::QtWidgetsItem item(widget);
    auto bounds = item.bounds();

    EXPECT_EQ(bounds.size.width, 200.0);
    EXPECT_EQ(bounds.size.height, 100.0);

    auto expectedPos = widget->mapToGlobal(QPoint(0, 0));
    EXPECT_EQ(bounds.topLeft.x, static_cast<double>(expectedPos.x()));
    EXPECT_EQ(bounds.topLeft.y, static_cast<double>(expectedPos.y()));

    delete widget;
}

TEST_F(QtWidgetsItemTest, VisibleReturnsTrueWhenVisible)
{
    auto widget = CreateTestWidget();
    widget->show();

    spix::QtWidgetsItem item(widget);
    EXPECT_TRUE(item.visible());

    delete widget;
}

TEST_F(QtWidgetsItemTest, VisibleReturnsFalseWhenHidden)
{
    auto widget = CreateTestWidget();
    widget->hide();

    spix::QtWidgetsItem item(widget);
    EXPECT_FALSE(item.visible());

    delete widget;
}

TEST_F(QtWidgetsItemTest, StringPropertyReturnsLabelText)
{
    auto [widget, label] = CreateWidgetWithLabel();
    label->setText("Hello World");

    spix::QtWidgetsItem item(label);
    auto text = item.stringProperty("text");

    EXPECT_EQ(text, "Hello World");

    delete widget;
}

TEST_F(QtWidgetsItemTest, SetStringPropertyUpdatesLabelText)
{
    auto [widget, label] = CreateWidgetWithLabel();

    spix::QtWidgetsItem item(label);
    item.setStringProperty("text", "Updated Text");

    EXPECT_EQ(label->text(), "Updated Text");

    delete widget;
}

TEST_F(QtWidgetsItemTest, StringPropertyReturnsObjectName)
{
    auto widget = CreateTestWidget("myWidget");

    spix::QtWidgetsItem item(widget);
    auto name = item.stringProperty("objectName");

    EXPECT_EQ(name, "myWidget");

    delete widget;
}

TEST_F(QtWidgetsItemTest, QWidgetReturnsInternalWidget)
{
    auto widget = CreateTestWidget();

    spix::QtWidgetsItem item(widget);
    EXPECT_EQ(item.qwidget(), widget);

    delete widget;
}

TEST_F(QtWidgetsItemTest, ConstQWidgetReturnsInternalWidget)
{
    auto widget = CreateTestWidget();

    const spix::QtWidgetsItem item(widget);
    EXPECT_EQ(item.qwidget(), widget);

    delete widget;
}
