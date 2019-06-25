/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>

namespace uibot {
namespace cmd {

class ClickOnItem : public Command {
public:
    ClickOnItem(ItemPath path, bool press = true, bool release = true);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    bool m_press;
    bool m_release;
};

} // namespace cmd
} // namespace uibot
