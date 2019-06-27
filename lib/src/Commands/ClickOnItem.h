/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPosition.h>

namespace spix {
namespace cmd {

class ClickOnItem : public Command {
public:
    ClickOnItem(ItemPosition path);

    void execute(CommandEnvironment& env) override;

private:
    ItemPosition m_position;
};

} // namespace cmd
} // namespace spix
