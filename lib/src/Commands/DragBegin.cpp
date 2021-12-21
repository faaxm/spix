/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DragBegin.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

DragBegin::DragBegin(ItemPath path)
: m_path(std::move(path))
{
}

void DragBegin::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (!item) {
        env.state().reportError("DragBegin: Item not found: " + m_path.string());
        return;
    }

    auto size = item->size();
    Point midPoint(size.width / 2.0, size.height / 2.0);
    env.scene().events().mouseDown(item.get(), midPoint, MouseButtons::Left);
    env.scene().events().mouseMove(item.get(), midPoint);
}

} // namespace cmd
} // namespace spix
