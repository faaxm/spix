/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/QtWidgetsBot.h>

#include <QtDebug>
#include <QtWidgetsScene.h>
#include <Spix/CommandExecuter/CommandExecuter.h>
#include <Spix/TestServer.h>

namespace spix {

QtWidgetsBot::QtWidgetsBot(QObject* parent)
: QObject(parent)
, m_scene(std::make_unique<QtWidgetsScene>())
, m_cmdExec(std::make_unique<CommandExecuter>())
{
    startTimer(10);
}

QtWidgetsBot::~QtWidgetsBot() = default;

void QtWidgetsBot::runTestServer(TestServer& server)
{
    qInfo() << "Spix server is enabled. Only use this in a safe environment.";
    server.setCommandExecuter(m_cmdExec.get());
    server.start();
}

void QtWidgetsBot::timerEvent(QTimerEvent*)
{
    m_cmdExec->processCommands(*m_scene);
}

} // namespace spix
