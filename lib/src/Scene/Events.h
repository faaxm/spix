/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/PasteboardContent.h>

#include <string>

namespace uibot {

class Events {
public:
    virtual ~Events() = default;

    virtual void mouseClick(Item* item, Point loc, bool press = true, bool release = true) = 0;
    virtual void stringInput(Item* item, const std::string& text) = 0;
    virtual void extMouseDrop(Item* item, Point loc, PasteboardContent& content) = 0;
    virtual void quit() = 0;
};

} // namespace uibot
