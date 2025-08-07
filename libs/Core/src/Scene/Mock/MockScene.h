/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Scene/Mock/MockEvents.h>
#include <Scene/Mock/MockItem.h>
#include <Spix/Scene/Scene.h>
#include <map>

namespace spix {

class SPIXCORE_EXPORT MockScene : public Scene {
public:
    // Request objects
    std::unique_ptr<Item> itemAtPath(const ItemPath& path) override;

    // Events
    Events& events() override;

    // Tasks
    void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) override;
    std::string takeScreenshotAsBase64(const ItemPath& targetItem) override;
    // Mock stuff
    void addItemAtPath(MockItem item, const ItemPath& path);
    MockEvents& mockEvents();

private:
    std::map<std::string, MockItem> m_items;
    MockEvents m_events;
};

} // namespace spix
