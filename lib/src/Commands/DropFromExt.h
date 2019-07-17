/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>
#include <Spix/Data/PasteboardContent.h>
#include <future>

namespace spix {
namespace cmd {

class DropFromExt : public Command {
public:
    DropFromExt(ItemPath path, PasteboardContent content, std::promise<void> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    PasteboardContent m_content;
    std::promise<void> m_promise;
};

} // namespace cmd
} // namespace spix
