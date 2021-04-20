/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "GetBoundingBox.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

GetBoundingBox::GetBoundingBox(ItemPath path, std::promise<Rect> promise)
: m_path(std::move(path))
, m_promise(std::move(promise))
{
}

void GetBoundingBox::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (item) {
        auto value = item->bounds();
        m_promise.set_value(value);
    } else {
        m_promise.set_value(Rect {0.0, 0.0, 0.0, 0.0});
        env.state().reportError("GetBoundingBox: Item not found: " + m_path.string());
    }
}

} // namespace cmd
} // namespace spix
