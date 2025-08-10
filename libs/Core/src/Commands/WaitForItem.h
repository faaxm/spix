/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

#include <chrono>
#include <future>

namespace spix {
namespace cmd {

class WaitForItem : public Command {
public:
    WaitForItem(ItemPath path, std::chrono::milliseconds maxWaitTime, std::promise<bool> promise);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow(CommandEnvironment&) override;

private:
    bool m_timerInitialized = false;
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::milliseconds m_maxWaitTime;
    ItemPath m_path;
    std::promise<bool> m_promise;
    bool m_itemFound = false;
};

} // namespace cmd
} // namespace spix
