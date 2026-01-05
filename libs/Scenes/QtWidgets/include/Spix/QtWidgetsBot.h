/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QObject>
#include <Spix/TestServer.h>
#include <memory>

#include <Spix/spix_qtwidgets_export.h>

namespace spix {

class CommandExecuter;
class QtWidgetsScene;

/**
 * @brief Class that maintains and runs the test environment for QWidgets
 *
 * This QObject creates the test environment and hooks up
 * to the Qt timer to get a callback on the main thread and
 * process test commands.
 *
 * Usually it is enough to create one object of this type
 * in `main()` of your application to start processing tests.
 */
class SPIXQTWIDGETS_EXPORT QtWidgetsBot : public QObject {
    Q_OBJECT

public:
    QtWidgetsBot(QObject* parent = nullptr);
    ~QtWidgetsBot();

    void runTestServer(TestServer& server);

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    std::unique_ptr<QtWidgetsScene> m_scene;
    std::unique_ptr<CommandExecuter> m_cmdExec;
};

} // namespace spix
