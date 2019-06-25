/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "CustomCmd.h"

namespace uibot {
namespace cmd {

CustomCmd::CustomCmd(CustomCmd::ExecFunction exec, CustomCmd::CanExecFunction canExec)
: m_exec(exec)
, m_canExec(canExec)
{
}

void CustomCmd::execute(uibot::CommandEnvironment& env)
{
    m_exec(env);
}

bool CustomCmd::canExecuteNow()
{
    return m_canExec();
}

} // namespace cmd
} // namespace uibot
