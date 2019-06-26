/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Qt/QtEvents.h>
#include <Scene/Scene.h>

#include <map>
#include <string>

class QQuickWindow;

namespace spix {

class ItemPath;

class QtScene : public Scene {
public:
    // Request objects
    std::unique_ptr<Item> itemAtPath(const ItemPath& path) override;

    // Events
    Events& events() override;

    // Tasks
    void takeScreenshot(const ItemPath& targetItem, const std::string& filePath) override;

private:
    QtEvents m_events;
};

} // namespace uibot
