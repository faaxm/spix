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

#include <memory>
#include <string>

namespace spix {

class PasteboardContent;

/**
 * @brief The basic actions a scene can perform
 *
 * Each backend for different application types (Qt/Qml/Mock/...) has to
 * implement this to grant Commands access to the scene and its objects.
 * Due to this, all commands can be reused with varying backends.
 */
class Scene {
public:
    virtual ~Scene() = default;

    // Request objects
    virtual std::unique_ptr<Item> itemAtPath(const ItemPath& path) = 0;

    // Events
    virtual Events& events() = 0;

    // Tasks
    virtual void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) = 0;
    virtual std::string takeScreenshotAsBase64(const ItemPath& targetItem) = 0;
};

} // namespace spix
