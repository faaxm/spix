/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>

#include <iostream>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Init Qt Qml Application
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Instantiate and run tests
    uibot::AnyRpcServer server;
    auto bot = new uibot::QtQmlBot();
    bot->runTestServer(server);

    return app.exec();
}
