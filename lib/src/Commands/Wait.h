/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"

#include <chrono>

namespace uibot {
namespace cmd {

class Wait : public Command {
public:
    Wait(std::chrono::milliseconds waitTime);

    void execute(CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    bool m_timerInitialized = false;
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::milliseconds m_waitTime;
};

} // namespace cmd
} // namespace uibot
