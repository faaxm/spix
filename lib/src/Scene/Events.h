/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Item.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/PasteboardContent.h>
#include <Spix/Events/Identifiers.h>

#include <string>

namespace spix {

class Events {
public:
    virtual ~Events() = default;

    virtual void mouseDown(Item* item, Point loc, MouseButton button, KeyModifier mod) = 0;
    virtual void mouseUp(Item* item, Point loc, MouseButton button, KeyModifier mod) = 0;
    virtual void mouseMove(Item* item, Point loc) = 0;
    virtual void stringInput(Item* item, const std::string& text) = 0;
    virtual void keyPress(Item* item, int keyCode, KeyModifier mod) = 0;
    virtual void keyRelease(Item* item, int keyCode, KeyModifier mod) = 0;
    virtual void extMouseDrop(Item* item, Point loc, PasteboardContent& content) = 0;
    virtual void quit() = 0;
};

} // namespace spix
