/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtWidgetsBot.h>

#include <iostream>

class FormWindow : public QMainWindow {
public:
    FormWindow(QWidget* parent = nullptr)
    : QMainWindow(parent)
    {
        setObjectName("formWindow");
        setWindowTitle("Spix QtWidgets Form Example");

        auto centralWidget = new QWidget(this);
        auto layout = new QFormLayout(centralWidget);

        m_nameEdit = new QLineEdit(this);
        m_nameEdit->setObjectName("nameField");
        layout->addRow("Name:", m_nameEdit);

        m_roleCombo = new QComboBox(this);
        m_roleCombo->setObjectName("roleCombo");
        m_roleCombo->addItems({"Developer", "Designer", "Manager"});
        layout->addRow("Role:", m_roleCombo);

        m_activeCheck = new QCheckBox("Active", this);
        m_activeCheck->setObjectName("activeCheck");
        layout->addRow("Status:", m_activeCheck);

        m_submitBtn = new QPushButton("Submit", this);
        m_submitBtn->setObjectName("submitBtn");
        layout->addRow(m_submitBtn);

        m_statusLabel = new QLabel("", this);
        m_statusLabel->setObjectName("statusLabel");
        layout->addRow("Result:", m_statusLabel);

        setCentralWidget(centralWidget);

        connect(m_submitBtn, &QPushButton::clicked, this, [this]() {
            QString status = QString("Name: %1, Role: %2, Active: %3")
                                 .arg(m_nameEdit->text())
                                 .arg(m_roleCombo->currentText())
                                 .arg(m_activeCheck->isChecked() ? "Yes" : "No");
            m_statusLabel->setText(status);
        });
    }

private:
    QLineEdit* m_nameEdit;
    QComboBox* m_roleCombo;
    QCheckBox* m_activeCheck;
    QPushButton* m_submitBtn;
    QLabel* m_statusLabel;
};

class FormTests : public spix::TestServer {
protected:
    void executeTest() override
    {
        // Input name using inputText
        inputText(spix::ItemPath("formWindow/nameField"), "John Doe");
        wait(std::chrono::milliseconds(200));

        // Toggle checkbox
        mouseClick(spix::ItemPath("formWindow/activeCheck"));
        wait(std::chrono::milliseconds(100));

        // Submit
        mouseClick(spix::ItemPath("formWindow/submitBtn"));
        wait(std::chrono::milliseconds(200));

        auto result = getStringProperty("formWindow/statusLabel", "text");
        std::cout << "Form result: " << result << std::endl;

        quit();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    FormWindow window;
    window.show();

    FormTests tests;
    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(tests);

    return app.exec();
}
