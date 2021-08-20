/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include <Scene/Mock/MockEvents.h>
#include <Scene/Mock/MockItem.h>
#include <Scene/Scene.h>
#include <map>

namespace spix {

class SPIX_EXPORT MockScene : public Scene {
public:
    // Request objects
    std::unique_ptr<Item> itemAtPath(const ItemPath& path) override;

    // Events
    Events& events() override;

    // Tasks
    void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) override;

    // Mock stuff
    void addItemAtPath(MockItem item, const ItemPath& path);
    MockEvents& mockEvents();

private:
    std::map<std::string, MockItem> m_items;
    MockEvents m_events;
};

} // namespace spix
