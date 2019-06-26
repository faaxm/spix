/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>

namespace spix {
namespace cmd {

class ClickOnItem : public Command {
public:
    ClickOnItem(ItemPath path);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
};

} // namespace cmd
} // namespace uibot
