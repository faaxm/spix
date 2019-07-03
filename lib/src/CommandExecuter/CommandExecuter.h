/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <CommandExecuter/ExecuterState.h>
#include <Commands/Command.h>

#include <memory>
#include <thread>
#include <mutex>
#include <queue>

namespace spix {

class Scene;

/**
 * @brief Handles and processes commands
 *
 * The CommandExecuter manages a queue of commands that will
 * be processed on the main thread.
 *
 * Commands can be enqueued from any thread, but all other
 * methods have to be called from the main thread.
 */
class CommandExecuter {
public:
    CommandExecuter();

    ExecuterState& state();

    void enqueueCommand(std::unique_ptr<cmd::Command> command);
    void processCommands(Scene& scene);

    template <typename CmdType, typename... Args>
    void enqueueCommand(Args&&... args)
    {
        enqueueCommand(std::make_unique<CmdType>(std::forward<Args>(args)...));
    }

private:
    std::thread::id m_mainThreadId;
    std::mutex m_mutex;
    std::queue<std::unique_ptr<cmd::Command>> m_commandQueue;
    bool m_processingEnabled;
    ExecuterState m_state;
};

} // namespace spix
