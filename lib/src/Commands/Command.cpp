/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Command.h"

namespace spix {
namespace cmd {

bool Command::canExecuteNow(CommandEnvironment&)
{
    return true;
}

} // namespace cmd
} // namespace spix
