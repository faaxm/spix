/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include "Command.h"
#include <Scene/Events.h>
#include <Spix/Data/ItemPosition.h>
#include <chrono>

namespace spix {
namespace cmd {

class SPIX_EXPORT ClickAndHoldOnItem : public Command {
public:
    ClickAndHoldOnItem(ItemPosition path, std::chrono::milliseconds holdTime, MouseButton mouseButton);

    void execute(CommandEnvironment& env) override;
    bool canExecuteNow(CommandEnvironment& env) override;

private:
    void pressOrRelease(CommandEnvironment& env, bool isRelease);

private:
    ItemPosition m_position;
    MouseButton m_mouseButton;
    bool m_timerInitialized = false;
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::milliseconds m_holdTime;
};

} // namespace cmd
} // namespace spix
