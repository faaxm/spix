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

TEST(GTestExample, SearchTypeCentre)
{
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle/#Rectangle/\"Click Me\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(Button 2 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeCentreShortened)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle/\"Click Me\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(Button 2 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchWrongText)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "No Click");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle\"Not on the Screen\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(No Click)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeInTwoRects)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle/\"Click Me\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(Button 2 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeInTwoRectsWithWrongButton)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "No Click");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle/\"Click You\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(No Click)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeCentreFirstButton)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/\"Click Me\""));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(Button 1 clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeRectangles)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/ParentRect/#Rectangle"));
    srv->wait(std::chrono::milliseconds(500));

    auto result = srv->getStringProperty("mainWindow/results", "text");
    auto expected_result = R"RSLT(Rect clicked)RSLT";

    EXPECT_EQ(result, expected_result);
}

TEST(GTestExample, SearchTypeTooManyRectangles)
{
    // Clean Output
    srv->setStringProperty("mainWindow/results", "text", "No Click");
    srv->wait(std::chrono::milliseconds(500));
    // Search for mutible Types in a Path
    srv->mouseClick(spix::ItemPath("mainWindow/#Rectangle/#Rectangle/#Rectangle/#Rectangle"));
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
