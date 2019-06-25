/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <CommandExecuter/CommandEnvironment.h>

namespace uibot {
namespace cmd {

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(CommandEnvironment& env) = 0;
    virtual bool canExecuteNow();
};

} // namespace cmd
} // namespace uibot
