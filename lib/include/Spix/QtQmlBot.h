/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QObject>
#include <Spix/TestServer.h>
#include <memory>

namespace spix {

class CommandExecuter;
class QtScene;

/**
 * @brief Class that maintains and runs the test environment
 *
 * This QObject creates the test environment and hooks up
 * to the qt timer to get a callback on the main thread and
 * process test commands.
 *
 * Usually it is enough to create one object of this type
 * in `main()` of your application to start processing tests.
 */
class QtQmlBot : public QObject {
    Q_OBJECT

public:
    QtQmlBot(QObject* parent = nullptr);
    ~QtQmlBot();

    void runTestServer(TestServer& server);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    std::unique_ptr<QtScene> m_scene;
    std::unique_ptr<CommandExecuter> m_cmdExec;
};

} // namespace uibot
