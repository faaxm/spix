/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Commands/Command.h>
#include <functional>

namespace spix {

namespace cmd {

class SPIXCORE_EXPORT CustomCmd : public spix::cmd::Command {
public:
    using ExecFunction = std::function<void(spix::CommandEnvironment&)>;
    using CanExecFunction = std::function<bool()>;

    CustomCmd(ExecFunction exec, CanExecFunction canExec);

    void execute(spix::CommandEnvironment&) override;
    bool canExecuteNow(CommandEnvironment&) override;

private:
    ExecFunction m_exec;
    CanExecFunction m_canExec;
};

} // namespace cmd
} // namespace spix
