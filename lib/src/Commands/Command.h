/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include <CommandExecuter/CommandEnvironment.h>

namespace spix {
namespace cmd {

class SPIX_EXPORT Command {
public:
    virtual ~Command() = default;

    virtual void execute(CommandEnvironment& env) = 0;
    virtual bool canExecuteNow(CommandEnvironment&);
};

} // namespace cmd
} // namespace spix
