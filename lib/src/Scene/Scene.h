/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Events.h>
#include <Scene/Item.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/ItemPath.h>

#include <string>

namespace spix {

class PasteboardContent;

class Scene {
public:
    virtual ~Scene() = default;

    // Request objects
    virtual std::unique_ptr<Item> itemAtPath(const ItemPath& path) = 0;

    // Events
    virtual Events& events() = 0;

    // Tasks
    virtual void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) = 0;
};

} // namespace uibot
