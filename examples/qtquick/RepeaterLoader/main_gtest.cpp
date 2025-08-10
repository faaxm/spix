/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>

#include <atomic>
#include <gtest/gtest.h>
#include <iostream>

class SpixGTest;
static SpixGTest* srv;

class SpixGTest : public spix::TestServer {
public:
    SpixGTest(int argc, char* argv[])
    {
        m_argc = argc;
        m_argv = argv;
    }

    int testResult() { return m_result.load(); }

protected:
    int m_argc;
    char** m_argv;
    std::atomic<int> m_result {0};

    void executeTest() override
    {
        srv = this;
        ::testing::InitGoogleTest(&m_argc, m_argv);
        auto testResult = RUN_ALL_TESTS();
        m_result.store(testResult);
    }
};

TEST(RepeaterLoaderGTest, RepeaterLoaderTest)
{
    EXPECT_FALSE(srv->existsAndVisible("mainWindow/ItemDisplayLoader/textItem"))
        << "Error: Item should not be visible at launch.";

    srv->mouseClick("mainWindow/ItemButtons/Item_0");
    srv->wait(std::chrono::milliseconds(500));

    EXPECT_TRUE(srv->existsAndVisible("mainWindow/ItemDisplayLoader/textItem"))
        << "Error: Item should be displayed after button pressed.";

    // 'ItemButtons' is not required in the path,
    // as 'gridItem_0' is unique within 'mainWindow'
    srv->mouseClick("mainWindow/Item_1");
    srv->wait(std::chrono::milliseconds(500));

    EXPECT_STREQ(srv->getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text").c_str(),
        "I am a view with a pear. Trust me.")
        << "Error: Wrong item displayed.";

    srv->mouseClick("mainWindow/ItemButtons/Item_2");
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick("mainWindow/ItemButtons/Item_0");
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick("mainWindow/ItemButtons/Item_3");
    srv->wait(std::chrono::milliseconds(100));

    EXPECT_STREQ(srv->getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text").c_str(),
        "I am a view with a cucumber. Trust me.")
        << "Error: Wrong item displayed";

    srv->mouseClick("mainWindow/itemCombo");
    srv->wait(std::chrono::milliseconds(400));

    srv->enterKey("mainWindow/itemCombo", spix::KeyCodes::Down, 0); // Down
    srv->wait(std::chrono::milliseconds(200));
    srv->enterKey("mainWindow/itemCombo", spix::KeyCodes::Down, 0); // Down
    srv->wait(std::chrono::milliseconds(200));
    srv->enterKey("mainWindow/itemCombo", spix::KeyCodes::Enter, 0); // Enter
    srv->wait(std::chrono::milliseconds(100));

    EXPECT_STREQ(srv->getStringProperty("mainWindow/ItemDisplayLoader/textItem", "text").c_str(),
        "I am a view with a banana. Trust me.")
        << "Error: Wrong item displayed";

    srv->quit();
}

int main(int argc, char* argv[])
{
    // Init Qt Qml Application
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Instantiate and run tests
    SpixGTest tests(argc, argv);
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);

    app.exec();
    return tests.testResult();
}
