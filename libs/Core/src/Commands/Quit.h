/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>

namespace spix {
namespace cmd {

class Quit : public Command {
public:
    void execute(CommandEnvironment& env) override;
};

} // namespace cmd
} // namespace spix
