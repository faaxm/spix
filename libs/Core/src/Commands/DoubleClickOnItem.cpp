/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DoubleClickOnItem.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

DoubleClickOnItem::DoubleClickOnItem(ItemPosition position, MouseButton mouseButton, KeyModifier keyModifier)
: m_position(std::move(position))
, m_mouseButton(mouseButton)
, m_keyModifier(keyModifier)
{
}

void DoubleClickOnItem::execute(CommandEnvironment& env)
{
    if (m_mouseButton != MouseButtons::Left && m_mouseButton != MouseButtons::Right
        && m_mouseButton != MouseButtons::Middle) {
        env.state().reportError("DoubleClickOnItem: Expected one left, right, or middle mouse button");
        return;
    }

    constexpr KeyModifier supportedModifiers
        = KeyModifiers::Shift | KeyModifiers::Control | KeyModifiers::Alt | KeyModifiers::Meta;
    if (m_keyModifier & ~supportedModifiers) {
        env.state().reportError("DoubleClickOnItem: Unsupported key modifiers");
        return;
    }

    const auto& path = m_position.itemPath();
    auto item = env.scene().itemAtPath(path);
    if (!item) {
        env.state().reportError("DoubleClickOnItem: Item not found: " + path.string());
        return;
    }

    auto mousePoint = m_position.positionForItemSize(item->size());
    if (!env.scene().events().mouseDoubleClick(item.get(), mousePoint, m_mouseButton, m_keyModifier)) {
        env.state().reportError("DoubleClickOnItem: Backend could not queue the gesture: " + path.string());
    }
}

} // namespace cmd
} // namespace spix
