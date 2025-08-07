/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/CommandExecuter/CommandEnvironment.h>

namespace spix {
namespace cmd {

class SPIXCORE_EXPORT Command {
public:
    virtual ~Command() = default;

    virtual void execute(CommandEnvironment& env) = 0;
    virtual bool canExecuteNow(CommandEnvironment&);
};

} // namespace cmd
} // namespace spix
