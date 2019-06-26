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

class Screenshot : public Command {
public:
    Screenshot(ItemPath targetItemPath, std::string filePath);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_itemPath;
    std::string m_filePath;
};

} // namespace cmd
} // namespace spix
