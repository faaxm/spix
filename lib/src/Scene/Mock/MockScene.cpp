/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "MockScene.h"
#include <Scene/Mock/MockItem.h>

namespace spix {

std::unique_ptr<Item> MockScene::itemAtPath(const ItemPath& path)
{
    auto foundItem = m_items.find(path.string());
    if (foundItem != m_items.end()) {
        return std::make_unique<MockItem>(foundItem->second);
    }

    return std::unique_ptr<MockItem>();
}

Events& MockScene::events()
{
    return m_events;
}

void MockScene::takeScreenshot(const ItemPath&, const std::string&)
{
}

void MockScene::addItemAtPath(MockItem item, const ItemPath& path)
{
    m_items.emplace(std::make_pair(path.string(), std::move(item)));
}

MockEvents& MockScene::mockEvents()
{
    return m_events;
}

} // namespace uibot
