/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Scene/Events.h>
#include <Spix/Data/ItemPath.h>

namespace spix {
namespace cmd {

class EnterKey : public Command {
public:
    EnterKey(ItemPath path, int keyCode, KeyModifier mod);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    int m_keyCode;
    KeyModifier m_mod;
};

} // namespace cmd
} // namespace spix
