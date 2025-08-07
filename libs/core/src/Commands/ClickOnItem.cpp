/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "ClickOnItem.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

ClickOnItem::ClickOnItem(ItemPosition path, MouseButton mouseButton, KeyModifier keyModifier)
: m_position(std::move(path))
, m_mouseButton(mouseButton)
, m_keyModifier(keyModifier)
{
}

void ClickOnItem::execute(CommandEnvironment& env)
{
    auto path = m_position.itemPath();
    auto item = env.scene().itemAtPath(path);

    if (!item) {
        env.state().reportError("ClickOnItem: Item not found: " + path.string());
        return;
    }

    auto size = item->size();
    auto mousePoint = m_position.positionForItemSize(size);
    env.scene().events().mouseDown(item.get(), mousePoint, m_mouseButton, m_keyModifier);
    env.scene().events().mouseUp(item.get(), mousePoint, m_mouseButton, m_keyModifier);
}

} // namespace cmd
} // namespace spix
