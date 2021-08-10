/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "spix_export.h"

#include "Command.h"
#include <Spix/Data/ItemPath.h>
#include <Spix/Data/PasteboardContent.h>

namespace spix {
namespace cmd {

class SPIX_EXPORT DropFromExt : public Command {
public:
    DropFromExt(ItemPath path, PasteboardContent content);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    PasteboardContent m_content;
};

} // namespace cmd
} // namespace spix
