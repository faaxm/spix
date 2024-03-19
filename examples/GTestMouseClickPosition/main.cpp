/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

/**
 * This is a very basic example to demonstrate how to run your UI tests
 * using GTest. It can be useful when you have to make sure that your
 * UI tests work well in an existing, GTest based environment.
 *
 * Keep in mind that GTest is not designed for UI testing and that the
 * order of the test execution is not guaranteed. Thus, you should only
 * have one test per executable.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Spix/Events/Identifiers.h>
#include <Spix/QtQmlBot.h>

#include <atomic>
#include <gtest/gtest.h>

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

TEST(GTestExample, ProportionAllButtons)
{
    const auto upperLeft = spix::Point(0.25, 0.25);
    const auto upperRigth = spix::Point(0.75, 0.25);
    const auto lowerLeft = spix::Point(0.25, 0.75);
    const auto lowerRigth = spix::Point(0.75, 0.75);

    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), upperLeft);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), upperRigth);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), lowerLeft);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), lowerRigth);
    srv->wait(std::chrono::milliseconds(500));
    auto result = srv->getStringProperty("mainWindow/results", "text");

    auto expected_result = R"RSLT(Button 1 clicked
Button 2 clicked
Button 3 clicked
Button 4 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, ToMuchProportion)
{
    // This Test click 200% under the Button 1 => under the Button 1 is Button 3  which will trigger.
    const auto upperLeft = spix::Point(0.25, 2);

    srv->setStringProperty("mainWindow/results", "text", "");
    srv->mouseClick(spix::ItemPath("mainWindow/Button_1"), upperLeft);
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");

    auto expected_result = R"RSLT(Button 3 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, OffsetAllButtons)
{
    const auto upperLeft = spix::Point(100, 100);
    const auto upperRigth = spix::Point(400, 100);
    const auto lowerLeft = spix::Point(100, 200);
    const auto lowerRigth = spix::Point(400, 200);

    srv->setStringProperty("mainWindow/results", "text", "");
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), spix::Point(0, 0), upperLeft);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), spix::Point(0, 0), upperRigth);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), spix::Point(0, 0), lowerLeft);
    srv->wait(std::chrono::milliseconds(500));
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), spix::Point(0, 0), lowerRigth);
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");

    auto expected_result = R"RSLT(Button 1 clicked
Button 2 clicked
Button 3 clicked
Button 4 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, OffsetOnPositionWithNoButton)
{
    const auto centre = spix::Point(320, 120);

    srv->setStringProperty("mainWindow/results", "text", "No Click");
    srv->mouseClick(spix::ItemPath("mainWindow/Grid_1"), spix::Point(0, 0), centre);
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");

    auto expected_result = R"RSLT(No Click)RSLT";

    EXPECT_EQ(result, expected_result);

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
