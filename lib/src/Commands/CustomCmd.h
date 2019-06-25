/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <functional>

namespace uibot {

namespace cmd {

class CustomCmd : public uibot::cmd::Command {
public:
    using ExecFunction = std::function<void(uibot::CommandEnvironment&)>;
    using CanExecFunction = std::function<bool()>;

    CustomCmd(ExecFunction exec, CanExecFunction canExec);

    void execute(uibot::CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ExecFunction m_exec;
    CanExecFunction m_canExec;
};

} // namespace cmd
} // namespace uibot
