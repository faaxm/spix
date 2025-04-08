/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPosition.h>
#include <Spix/Scene/Events.h>

namespace spix {
namespace cmd {

class SPIX_EXPORT ClickOnItem : public Command {
public:
    ClickOnItem(ItemPosition path, MouseButton mouseButton);

    void execute(CommandEnvironment& env) override;

private:
    ItemPosition m_position;
    MouseButton m_mouseButton;
};

} // namespace cmd
} // namespace spix
