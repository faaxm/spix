/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>
#include <Spix/Data/PasteboardContent.h>

namespace spix {
namespace cmd {

class SPIXCORE_EXPORT DropFromExt : public Command {
public:
    DropFromExt(ItemPath path, PasteboardContent content);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    PasteboardContent m_content;
};

} // namespace cmd
} // namespace spix
