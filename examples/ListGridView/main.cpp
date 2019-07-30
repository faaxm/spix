/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Spix/QtQmlBot.h>

#include <iostream>

class MyTests : public spix::TestServer {
protected:
    void executeTest() override
    {
        mouseClick(spix::ItemPath("mainWindow/myListView/listItem_0"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/myListView/listItem_2"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/myListView/listItem_1"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/myGridView/gridItem_3"));
        wait(std::chrono::milliseconds(500));

        // 'myGridView' is not required in the path,
        // as 'gridItem_0' is unique within 'mainWindow'
        mouseClick(spix::ItemPath("mainWindow/gridItem_0"));
        wait(std::chrono::milliseconds(500));

        mouseClick(spix::ItemPath("mainWindow/myListView/listItem_4"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/myGridView/gridItem_2"));
        wait(std::chrono::milliseconds(500));

        auto result = getStringProperty("mainWindow/results", "text");
        std::cout << "-------\nResult:\n-------\n" << result << "\n-------" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    // Init Qt Qml Application
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Instantiate and run tests
    MyTests tests;
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);

    return app.exec();
}
