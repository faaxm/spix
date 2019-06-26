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

class InputText : public Command {
public:
    InputText(ItemPath path, std::string text);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::string m_text;
};

} // namespace cmd
} // namespace uibot
