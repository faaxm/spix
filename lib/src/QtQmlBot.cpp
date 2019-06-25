/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/QtQmlBot.h>

#include <CommandExecuter/CommandExecuter.h>
#include <Scene/Qt/QtScene.h>
#include <Spix/TestServer.h>

namespace uibot {

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
    server.setCommandExecuter(m_cmdExec.get());
    server.start();
}

void QtQmlBot::timerEvent(QTimerEvent*)
{
    m_cmdExec->processCommands(*m_scene);
}

} // namespace uibot
