/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPosition.h>
#include <Spix/Scene/Events.h>

namespace spix {
namespace cmd {

class SPIXCORE_EXPORT ClickOnItem : public Command {
public:
    ClickOnItem(ItemPosition path, MouseButton mouseButton, KeyModifier keyModifier = KeyModifiers::None);

    void execute(CommandEnvironment& env) override;

private:
    ItemPosition m_position;
    MouseButton m_mouseButton;
    KeyModifier m_keyModifier;
};

} // namespace cmd
} // namespace spix
