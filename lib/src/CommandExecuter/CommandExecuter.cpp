/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "CommandExecuter.h"
#include "CommandEnvironment.h"

namespace spix {

CommandExecuter::CommandExecuter()
: m_commandQueue()
, m_processingEnabled(true)
{
}

ExecuterState& CommandExecuter::state()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_state;
}

void CommandExecuter::enqueueCommand(std::unique_ptr<cmd::Command> command)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_commandQueue.emplace(std::move(command));
}

void CommandExecuter::processCommands(Scene& scene)
{
    std::unique_lock<std::mutex> lock(m_mutex, std::try_to_lock);
    if (!lock) {
        return;
    }

    if (!m_processingEnabled) {
        return;
    }

    CommandEnvironment env(scene, m_state);

    while (!m_commandQueue.empty()) {
        auto& queuedCmd = m_commandQueue.front();

        if (!queuedCmd->canExecuteNow())
            break;

        // We can execute the command now.
        // Remove from queue and execute.
        std::unique_ptr<cmd::Command> localCmd = std::move(queuedCmd);
        m_commandQueue.pop();

        lock.unlock();
        localCmd->execute(env);
        if (!lock.try_lock()) {
            return;
        }
    }
}

} // namespace spix
