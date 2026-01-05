/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtWidgetsBot.h>

#include <iostream>

// Simple widget-based UI built purely in C++
class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr)
    : QMainWindow(parent)
    {
        setObjectName("mainWindow");
        setWindowTitle("Spix QtWidgets Basic Example");

        auto centralWidget = new QWidget(this);
        auto layout = new QVBoxLayout(centralWidget);

        m_button1 = new QPushButton("Button 1", this);
        m_button1->setObjectName("Button_1");

        m_button2 = new QPushButton("Button 2", this);
        m_button2->setObjectName("Button_2");

        m_results = new QLabel("", this);
        m_results->setObjectName("results");

        layout->addWidget(m_button1);
        layout->addWidget(m_button2);
        layout->addWidget(m_results);

        setCentralWidget(centralWidget);

        connect(m_button1, &QPushButton::clicked, this, [this]() { appendResult("Button 1 clicked"); });

        connect(m_button2, &QPushButton::clicked, this, [this]() { appendResult("Button 2 clicked"); });
    }

private:
    void appendResult(const QString& text)
    {
        QString current = m_results->text();
        if (!current.isEmpty())
            current += "\n";
        m_results->setText(current + text);
    }

    QPushButton* m_button1;
    QPushButton* m_button2;
    QLabel* m_results;
};

class MyTests : public spix::TestServer {
protected:
    void executeTest() override
    {
        mouseClick(spix::ItemPath("mainWindow/Button_1"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/Button_2"));
        wait(std::chrono::milliseconds(500));
        mouseClick(spix::ItemPath("mainWindow/Button_1"));
        wait(std::chrono::milliseconds(500));

        auto result = getStringProperty("mainWindow/results", "text");
        std::cout << "-------\nResult:\n-------\n" << result << "\n-------" << std::endl;

        quit();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    MyTests tests;
    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(tests);

    return app.exec();
}
