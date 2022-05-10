/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class Screenshot : public Command {
public:
    Screenshot(ItemPath targetItemPath, std::string filePath, std::promise<bool> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_itemPath;
    std::string m_filePath;
    std::promise<bool> m_promise;
};

} // namespace cmd
} // namespace spix
