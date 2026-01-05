/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>

#include <gtest/gtest.h>

#include <QApplication>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QtTest/QSignalSpy>

class WidgetTest : public ::testing::Test {
protected:
    WidgetTest()
    : fakeArg {"hello"}
    , fakeArgPtr(fakeArg)
    , fakeArgList(&fakeArgPtr)
    , fakeArgCount(1)
    , app(fakeArgCount, fakeArgList)
    {
    }

    /**
     * @brief Creates a simple test widget with common properties
     */
    QWidget* CreateTestWidget(const QString& objectName = "testWidget")
    {
        auto widget = new QWidget();
        widget->setObjectName(objectName);
        widget->resize(200, 100);
        return widget;
    }

    /**
     * @brief Creates a widget with a button that emits a signal when clicked
     */
    std::tuple<QWidget*, QPushButton*, std::unique_ptr<QSignalSpy>> CreateWidgetWithButton(
        const QString& buttonName = "testButton")
    {
        auto widget = new QWidget();
        widget->setObjectName("testWidget");
        auto layout = new QVBoxLayout(widget);

        auto button = new QPushButton("Click Me", widget);
        button->setObjectName(buttonName);
        layout->addWidget(button);

        auto spy = std::make_unique<QSignalSpy>(button, &QPushButton::clicked);
        if (!spy->isValid()) {
            throw std::runtime_error("Failed to create signal spy for button click");
        }

        return std::make_tuple(widget, button, std::move(spy));
    }

    /**
     * @brief Creates a widget with a label for property testing
     */
    std::tuple<QWidget*, QLabel*> CreateWidgetWithLabel(const QString& labelName = "testLabel")
    {
        auto widget = new QWidget();
        widget->setObjectName("testWidget");
        auto layout = new QVBoxLayout(widget);

        auto label = new QLabel("Test Label", widget);
        label->setObjectName(labelName);
        layout->addWidget(label);

        return std::make_tuple(widget, label);
    }

    char fakeArg[50];
    char* fakeArgPtr;
    char** fakeArgList;
    int fakeArgCount;
    QApplication app;
};
