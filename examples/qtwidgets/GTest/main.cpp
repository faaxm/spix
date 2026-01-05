/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtWidgetsBot.h>

#include <atomic>
#include <gtest/gtest.h>

class SpixGTest;
static SpixGTest* srv;

// Main window with input field and submit button
class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr)
    : QMainWindow(parent)
    {
        setObjectName("mainWindow");
        setWindowTitle("Spix QtWidgets GTest Example");

        auto centralWidget = new QWidget(this);
        auto layout = new QVBoxLayout(centralWidget);

        m_input = new QLineEdit(this);
        m_input->setObjectName("inputField");

        m_submitBtn = new QPushButton("Submit", this);
        m_submitBtn->setObjectName("submitButton");

        m_output = new QLabel("", this);
        m_output->setObjectName("outputLabel");

        layout->addWidget(m_input);
        layout->addWidget(m_submitBtn);
        layout->addWidget(m_output);

        setCentralWidget(centralWidget);

        connect(m_submitBtn, &QPushButton::clicked, this, [this]() { m_output->setText("Submitted: " + m_input->text()); });
    }

private:
    QLineEdit* m_input;
    QPushButton* m_submitBtn;
    QLabel* m_output;
};

class SpixGTest : public spix::TestServer {
public:
    SpixGTest(int argc, char* argv[])
    : m_argc(argc)
    , m_argv(argv)
    {
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

TEST(QtWidgetsGTest, BasicInputTest)
{
    // Type text into the input field
    srv->inputText(spix::ItemPath("mainWindow/inputField"), "Hello Spix!");
    srv->wait(std::chrono::milliseconds(200));

    // Click the submit button
    srv->mouseClick(spix::ItemPath("mainWindow/submitButton"));
    srv->wait(std::chrono::milliseconds(200));

    // Verify the output
    auto result = srv->getStringProperty("mainWindow/outputLabel", "text");
    EXPECT_EQ(result, "Submitted: Hello Spix!");

    srv->quit();
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    SpixGTest tests(argc, argv);
    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(tests);

    app.exec();
    return tests.testResult();
}
