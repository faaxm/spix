/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QByteArray>
#include <Spix/Data/ItemPath.h>
#include <Spix/Scene/Scene.h>
#include <Scene/Qt/QtEvents.h>

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
    std::string takeScreenshotAsBase64(const ItemPath& targetItem) override;

private:
    QtEvents m_events;
};

} // namespace spix
