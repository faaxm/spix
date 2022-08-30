/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>

#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QtTest/QSignalSpy>

class QMLEngineTest : public ::testing::Test {
protected:
    QMLEngineTest()
    : fakeArg {"hello"}
    , fakeArgPtr(fakeArg)
    , fakeArgList(&fakeArgPtr)
    , fakeArgCount(1)
    , app(fakeArgCount, fakeArgList)
    , engine()
    , component(&engine)
    {
    }

    QQuickItem* GetQQuickItemWithMethod(const char* methodBody)
    {
        std::string body = std::string("import QtQuick 2\n") + "Item {\n" + methodBody + "\n}\n";
        component.setData(QByteArray::fromStdString(body), QUrl());
        auto childItem = component.create();
        if (childItem == nullptr || !dynamic_cast<QQuickItem*>(childItem))
            throw std::runtime_error(
                std::string("Failed to create QML component: ") + component.errorString().toStdString());
        return static_cast<QQuickItem*>(childItem);
    }

    std::tuple<QQuickItem*, std::unique_ptr<QSignalSpy>> GetQQuickItemWithSignalSpy(
        const char* signalDef, const char* signalType)
    {
        QQuickItem* obj = GetQQuickItemWithMethod(signalDef);
        auto spy = std::make_unique<QSignalSpy>(obj, signalType);
        if (!spy->isValid()) {
            std::string sigreport;
            for (size_t i = 0; i < obj->metaObject()->methodCount(); i++)
                sigreport += obj->metaObject()->method(i).methodSignature().toStdString() + ", ";

            throw std::runtime_error(
                std::string("Failed to spy on signal: ") + signalType + ", all signals: " + sigreport);
        }

        return std::make_tuple(obj, std::move(spy));
    }

    char fakeArg[50];
    char* fakeArgPtr;
    char** fakeArgList;
    int fakeArgCount;
    QCoreApplication app;
    QQmlEngine engine;
    QQmlComponent component;
};
