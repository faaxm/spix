/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Scene/Events.h>
#include <Scene/Mock/MockItem.h>

#include <functional>

namespace spix {

class SPIXCORE_EXPORT MockEvents : public Events {
public:
    void mouseDown(Item* item, Point loc, MouseButton button, KeyModifier mod) override;
    void mouseUp(Item* item, Point loc, MouseButton button, KeyModifier mod) override;
    void mouseMove(Item* item, Point loc) override;
    void stringInput(Item* item, const std::string& text) override;
    void keyPress(Item* item, int keyCode, KeyModifier mod) override;
    void keyRelease(Item* item, int keyCode, KeyModifier mod) override;
    void extMouseDrop(Item* item, Point loc, PasteboardContent& content) override;
    void quit() override;

    // Mock stuff
    std::function<void(Item*, Point, bool, bool)> onMouseClickEvent;
    std::function<void(Item*, const std::string&)> onStringInputEvent;
    std::function<void(Item*, Point, PasteboardContent&)> onMouseDropEvent;
};

} // namespace spix
