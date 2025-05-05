/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "ClickAndHoldOnItem.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

ClickAndHoldOnItem::ClickAndHoldOnItem(ItemPosition path, std::chrono::milliseconds holdTime, MouseButton mouseButton)
: m_holdTime(holdTime)
, m_position(std::move(path))
, m_mouseButton(mouseButton)
{
}

void ClickAndHoldOnItem::execute(CommandEnvironment& env)
{
    pressOrRelease(env, true);
}

bool ClickAndHoldOnItem::canExecuteNow(CommandEnvironment& env)
{
    if (!m_timerInitialized) {
        m_timerInitialized = true;
        m_startTime = std::chrono::steady_clock::now();
        pressOrRelease(env, false);
        return false;
    }

    auto timeSinceStart = std::chrono::steady_clock::now() - m_startTime;
    return timeSinceStart >= m_holdTime;
}

void ClickAndHoldOnItem::pressOrRelease(CommandEnvironment& env, bool isRelease)
{
    auto path = m_position.itemPath();
    auto item = env.scene().itemAtPath(path);

    if (!item) {
        env.state().reportError("ClickAndHoldOnItem: Item not found: " + path.string());
        return;
    }

    auto size = item->size();
    auto mousePoint = m_position.positionForItemSize(size);
    if (!isRelease) {
        env.scene().events().mouseDown(item.get(), mousePoint, m_mouseButton);
    } else {
        env.scene().events().mouseUp(item.get(), mousePoint, m_mouseButton);
    }
}

} // namespace cmd
} // namespace spix
