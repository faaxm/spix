/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/QtQmlBot.h>

#include <QtDebug>
#include <QtScene.h>
#include <Spix/CommandExecuter/CommandExecuter.h>
#include <Spix/TestServer.h>

namespace spix {

QtQmlBot::QtQmlBot(QObject* parent)
: QObject(parent)
, m_scene(std::make_unique<QtScene>())
, m_cmdExec(std::make_unique<CommandExecuter>())
{
    startTimer(10);
}

QtQmlBot::~QtQmlBot() = default;

void QtQmlBot::runTestServer(TestServer& server)
{
    qInfo() << "Spix server is enabled. Only use this in a safe environment.";
    server.setCommandExecuter(m_cmdExec.get());
    server.start();
}

void QtQmlBot::timerEvent(QTimerEvent*)
{
    m_cmdExec->processCommands(*m_scene);
}

} // namespace spix
