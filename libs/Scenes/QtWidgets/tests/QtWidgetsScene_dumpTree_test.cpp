/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include "QtWidgetsTestUtils.h"
#include <QtWidgetsScene.h>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class QtWidgetsSceneDumpTreeTest : public WidgetTest {
protected:
    spix::QtWidgetsScene scene;
};

TEST_F(QtWidgetsSceneDumpTreeTest, EmptyPathReturnsEmptyJson)
{
    auto result = scene.dumpTree(spix::ItemPath(""));
    EXPECT_EQ(result, "{}");
}

TEST_F(QtWidgetsSceneDumpTreeTest, UnknownWidgetReturnsEmptyJson)
{
    auto result = scene.dumpTree(spix::ItemPath("nonExistentWidget"));
    EXPECT_EQ(result, "{}");
}

TEST_F(QtWidgetsSceneDumpTreeTest, TopLevelWidgetPathReturnsValidJson)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(result), &parseError);
    EXPECT_EQ(parseError.error, QJsonParseError::NoError) << result;
    EXPECT_TRUE(doc.isObject());
}

TEST_F(QtWidgetsSceneDumpTreeTest, TopLevelWidgetNodeHasExpectedFields)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    EXPECT_TRUE(root.contains("type"));
    EXPECT_TRUE(root.contains("objectName"));
    EXPECT_TRUE(root.contains("className"));
    EXPECT_TRUE(root.contains("bounds"));
    EXPECT_TRUE(root.contains("visible"));
    EXPECT_TRUE(root.contains("enabled"));
    EXPECT_TRUE(root.contains("children"));
    EXPECT_TRUE(root.contains("properties"));
}

TEST_F(QtWidgetsSceneDumpTreeTest, NodeNameMatchesObjectName)
{
    QWidget window;
    window.setObjectName("myNamedWindow");

    auto result = scene.dumpTree(spix::ItemPath("myNamedWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    EXPECT_EQ(root["objectName"].toString(), "myNamedWindow");
}

TEST_F(QtWidgetsSceneDumpTreeTest, BoundsContainWidthAndHeight)
{
    QWidget window;
    window.setObjectName("testWindow");
    window.resize(300, 150);

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    auto bounds = root["bounds"].toObject();
    EXPECT_EQ(bounds["width"].toDouble(), 300.0);
    EXPECT_EQ(bounds["height"].toDouble(), 150.0);
}

TEST_F(QtWidgetsSceneDumpTreeTest, ChildWidgetAppearsInJson)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto* button = new QPushButton("Click", &window);
    button->setObjectName("myButton");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    auto children = root["children"].toArray();
    ASSERT_FALSE(children.isEmpty());

    bool foundChild = false;
    for (const auto& childVal : children) {
        if (childVal.toObject()["objectName"].toString() == "myButton") {
            foundChild = true;
        }
    }
    EXPECT_TRUE(foundChild) << "Expected child named 'myButton' in JSON:\n" << result;
}

TEST_F(QtWidgetsSceneDumpTreeTest, DeepChildAppearsInJson)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto* container = new QWidget(&window);
    container->setObjectName("containerWidget");

    auto* label = new QLabel("hello", container);
    label->setObjectName("deepLabel");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();

    bool foundDeep = false;
    for (const auto& topVal : root["children"].toArray()) {
        auto topObj = topVal.toObject();
        if (topObj["objectName"].toString() == "containerWidget") {
            for (const auto& nestedVal : topObj["children"].toArray()) {
                if (nestedVal.toObject()["objectName"].toString() == "deepLabel") {
                    foundDeep = true;
                }
            }
        }
    }
    EXPECT_TRUE(foundDeep) << "Expected nested child 'deepLabel' in JSON:\n" << result;
}

TEST_F(QtWidgetsSceneDumpTreeTest, VisibilityReflectedInJson)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto* child = new QWidget(&window);
    child->setObjectName("hiddenWidget");
    child->hide();

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    for (const auto& childVal : root["children"].toArray()) {
        auto childObj = childVal.toObject();
        if (childObj["objectName"].toString() == "hiddenWidget") {
            EXPECT_FALSE(childObj["visible"].toBool());
        }
    }
}

TEST_F(QtWidgetsSceneDumpTreeTest, EnabledReflectedInJson)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto* button = new QPushButton("test", &window);
    button->setObjectName("disabledButton");
    button->setEnabled(false);

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    for (const auto& childVal : root["children"].toArray()) {
        auto childObj = childVal.toObject();
        if (childObj["objectName"].toString() == "disabledButton") {
            EXPECT_FALSE(childObj["enabled"].toBool());
        }
    }
}

TEST_F(QtWidgetsSceneDumpTreeTest, DeepPathResolvedToChildWidget)
{
    QWidget window;
    window.setObjectName("testWindow");

    auto* button = new QPushButton("test", &window);
    button->setObjectName("myButton");
    button->resize(80, 30);

    auto result = scene.dumpTree(spix::ItemPath("testWindow/myButton"));

    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(result), &parseError);
    EXPECT_EQ(parseError.error, QJsonParseError::NoError) << result;

    auto root = doc.object();
    EXPECT_EQ(root["objectName"].toString(), "myButton");
}
