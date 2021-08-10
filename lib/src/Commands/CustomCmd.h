/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "spix_export.h"

#include "Command.h"
#include <functional>

namespace spix {

namespace cmd {

class SPIX_EXPORT CustomCmd : public spix::cmd::Command {
public:
    using ExecFunction = std::function<void(spix::CommandEnvironment&)>;
    using CanExecFunction = std::function<bool()>;

    CustomCmd(ExecFunction exec, CanExecFunction canExec);

    void execute(spix::CommandEnvironment&) override;
    bool canExecuteNow() override;

private:
    ExecFunction m_exec;
    CanExecFunction m_canExec;
};

} // namespace cmd
} // namespace spix
