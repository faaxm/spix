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
#include <future>

namespace spix {
namespace cmd {

class SPIX_EXPORT ClickOnItem : public Command {
public:
    ClickOnItem(
        ItemPosition path, MouseButton mouseButton, std::chrono::milliseconds timeOut, std::promise<bool> promise);

    void execute(CommandEnvironment& env) override;
    bool canExecuteNow(CommandEnvironment&) override;

private:
    bool m_timerInitialized = false;
    std::chrono::steady_clock::time_point m_startTime;
    std::chrono::milliseconds m_timeOut;
    ItemPosition m_position;
    MouseButton m_mouseButton;
    std::promise<bool> m_promise;
    bool m_itemFound = false;
};

} // namespace cmd
} // namespace spix
