/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DropFromExt.h"

#include <Spix/CommandExecuter/CommandEnvironment.h>
#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

DropFromExt::DropFromExt(ItemPath path, PasteboardContent content)
: m_path(std::move(path))
, m_content(std::move(content))
{
}

void DropFromExt::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (!item) {
        env.state().reportError("DropFromExt: Item not found: " + m_path.string());
        return;
    }

    auto size = item->size();
    Point midPoint(size.width / 2.0, size.height / 2.0);
    env.scene().events().extMouseDrop(item.get(), midPoint, m_content);
}

} // namespace cmd
} // namespace spix
