/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

#include <Spix/Events/Identifiers.h>
#include <Spix/QtWidgetsBot.h>

#include <iostream>

class ConfirmDialog : public QDialog {
public:
    ConfirmDialog(QWidget* parent = nullptr)
    : QDialog(parent)
    {
        setObjectName("confirmDialog");
        setWindowTitle("Confirm");

        auto layout = new QVBoxLayout(this);

        auto label = new QLabel("Are you sure?", this);
        label->setObjectName("confirmLabel");
        layout->addWidget(label);

        auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        buttonBox->setObjectName("buttonBox");

        // Give buttons object names
        buttonBox->button(QDialogButtonBox::Ok)->setObjectName("okButton");
        buttonBox->button(QDialogButtonBox::Cancel)->setObjectName("cancelButton");

        layout->addWidget(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }
};

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr)
    : QMainWindow(parent)
    {
        setObjectName("mainWindow");
        setWindowTitle("Spix QtWidgets Dialog Example");

        auto centralWidget = new QWidget(this);
        auto layout = new QVBoxLayout(centralWidget);

        m_statusLabel = new QLabel("Ready", this);
        m_statusLabel->setObjectName("statusLabel");

        auto showDialogBtn = new QPushButton("Show Dialog", this);
        showDialogBtn->setObjectName("showDialogBtn");

        layout->addWidget(showDialogBtn);
        layout->addWidget(m_statusLabel);

        setCentralWidget(centralWidget);

        connect(showDialogBtn, &QPushButton::clicked, this, [this]() {
            ConfirmDialog dialog(this);
            if (dialog.exec() == QDialog::Accepted) {
                m_statusLabel->setText("Confirmed!");
            } else {
                m_statusLabel->setText("Cancelled");
            }
        });
    }

private:
    QLabel* m_statusLabel;
};

class DialogTests : public spix::TestServer {
protected:
    void executeTest() override
    {
        // Open the dialog
        mouseClick(spix::ItemPath("mainWindow/showDialogBtn"));
        wait(std::chrono::milliseconds(300));

        // Click OK in the dialog
        // The dialog becomes a top-level window, address it by its objectName
        mouseClick(spix::ItemPath("confirmDialog/okButton"));
        wait(std::chrono::milliseconds(200));

        // Check result
        auto result = getStringProperty("mainWindow/statusLabel", "text");
        std::cout << "Dialog result: " << result << std::endl;

        quit();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    DialogTests tests;
    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(tests);

    return app.exec();
}
