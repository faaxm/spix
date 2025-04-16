/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "WaitForItem.h"
#include <Scene/Scene.h>

namespace spix {
namespace cmd {

WaitForItem::WaitForItem(ItemPath path, std::chrono::milliseconds maxWaitTime, std::promise<bool> promise)
: m_path(std::move(path))
, m_promise(std::move(promise))
, m_maxWaitTime(std::move(maxWaitTime))
{
}

void WaitForItem::execute(CommandEnvironment& env)
{
    m_promise.set_value(m_itemFound);
}

bool WaitForItem::canExecuteNow(CommandEnvironment& env)
{
    if (!m_timerInitialized) {
        m_timerInitialized = true;
        m_startTime = std::chrono::steady_clock::now();
        return false;
    }

    auto item = env.scene().itemAtPath(m_path);
    if (item) {
        m_itemFound = item->visible();
        return true;
    }

    auto timeSinceStart = std::chrono::steady_clock::now() - m_startTime;
    return timeSinceStart >= m_maxWaitTime;
}

} // namespace cmd
} // namespace spix
