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

namespace spix {

class Events {
public:
    struct MouseButtons {
        static const unsigned none = 0;
        static const unsigned left = 1 << 0;
        static const unsigned right = 1 << 1;
        static const unsigned middle = 1 << 2;
    };
    using MouseButton = unsigned;

    virtual ~Events() = default;

    virtual void mouseDown(Item* item, Point loc, MouseButton button) = 0;
    virtual void mouseUp(Item* item, Point loc, MouseButton button) = 0;
    virtual void mouseMove(Item* item, Point loc) = 0;
    virtual void stringInput(Item* item, const std::string& text) = 0;
    virtual void extMouseDrop(Item* item, Point loc, PasteboardContent& content) = 0;
    virtual void quit() = 0;
};

} // namespace spix
