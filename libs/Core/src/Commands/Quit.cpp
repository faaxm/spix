/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Quit.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

void Quit::execute(CommandEnvironment& env)
{
    env.scene().events().quit();
}

} // namespace cmd
} // namespace spix
