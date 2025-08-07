/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

namespace spix {
namespace cmd {

class DragEnd : public Command {
public:
    DragEnd(ItemPath path);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
};

} // namespace cmd
} // namespace spix
