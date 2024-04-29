/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "ClickOnItem.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

ClickOnItem::ClickOnItem(
    ItemPosition path, MouseButton mouseButton, std::chrono::milliseconds timeOut, std::promise<bool> promise)
: m_position(std::move(path))
, m_mouseButton(mouseButton)
, m_timeOut(std::move(timeOut))
, m_promise(std::move(promise))
{
}

void ClickOnItem::execute(CommandEnvironment& env)
{
    auto path = m_position.itemPath();

    if (!m_itemFound) {
        env.state().reportError("ClickOnItem: Item not found: " + path.string());
        m_promise.set_value(false);
    } else {
        auto item = env.scene().itemAtPath(m_position.itemPath());
        auto size = item->size();
        auto mousePoint = m_position.positionForItemSize(size);
        env.scene().events().mouseDown(item.get(), mousePoint, m_mouseButton);
        env.scene().events().mouseUp(item.get(), mousePoint, m_mouseButton);
        m_promise.set_value(true);
    }
}

bool ClickOnItem::canExecuteNow(CommandEnvironment& env)
{
    if (!m_timerInitialized) {
        m_timerInitialized = true;
        m_startTime = std::chrono::steady_clock::now();
        return false;
    }

    auto item = env.scene().itemAtPath(m_position.itemPath());
    if (item) {
        m_itemFound = item->visible();
        return true;
    }

    auto timeSinceStart = std::chrono::steady_clock::now() - m_startTime;
    return timeSinceStart >= m_timeOut;
}

} // namespace cmd
} // namespace spix
