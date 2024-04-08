/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>

#include <iostream>

class MyTests : public spix::TestServer {
protected:
    void executeTest() override
    {
        if (existsAndVisible("mainWindow/ItemDisplayLoader/textItem")) {
            std::cout << "Error: Item should not be visible at launch." << std::endl;
        }

        mouseClick("mainWindow/ItemButtons/Item_0/mouseArea");
        wait(std::chrono::milliseconds(500));

        if (!existsAndVisible("mainWindow/ItemDisplayLoader/textItem")) {
            std::cout << "Error: Item should be displayed after button pressed." << std::endl;
        }

        // 'ItemButtons' is not required in the path,
        // as 'gridItem_0' is unique within 'mainWindow'
        mouseClick("mainWindow/Item_1/mouseArea");
        wait(std::chrono::milliseconds(500));

        auto result = getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text");
        if (result != "I am a view with a pear. Trust me.") {
            std::cout << "Error: Wrong item displayed." << std::endl;
        }

        mouseClick("mainWindow/ItemButtons/Item_2/mouseArea");
        wait(std::chrono::milliseconds(500));
        mouseClick("mainWindow/ItemButtons/Item_0/mouseArea");
        wait(std::chrono::milliseconds(500));
        mouseClick("mainWindow/ItemButtons/Item_3/mouseArea");
        wait(std::chrono::milliseconds(100));

        result = getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text");
        if (result != "I am a view with a cucumber. Trust me.") {
            std::cout << "Error: Wrong item displayed: \"" << result << "\"" << std::endl;
        }

        mouseClick("mainWindow/itemCombo");
        wait(std::chrono::milliseconds(400));

        enterKey("mainWindow/itemCombo", spix::KeyCodes::Down, 0); // Down
        wait(std::chrono::milliseconds(200));
        enterKey("mainWindow/itemCombo", spix::KeyCodes::Down, 0); // Down
        wait(std::chrono::milliseconds(200));
        enterKey("mainWindow/itemCombo", spix::KeyCodes::Enter, 0); // Enter
        wait(std::chrono::milliseconds(100));

        result = getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text");
        if (result != "I am a view with a banana. Trust me.") {
            std::cout << "Error: Wrong item displayed: \"" << result << "\"" << std::endl;
        }
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
