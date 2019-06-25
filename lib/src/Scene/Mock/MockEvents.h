/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Events.h>
#include <Scene/Mock/MockItem.h>

#include <functional>

namespace uibot {

class MockEvents : public Events {
public:
    void mouseClick(Item* item, Point loc, bool press = true, bool release = true) override;
    void stringInput(Item* item, const std::string& text) override;
    void extMouseDrop(Item* item, Point loc, PasteboardContent& content) override;
    void quit() override;

    // Mock stuff
    std::function<void(Item*, Point, bool, bool)> onMouseClickEvent;
    std::function<void(Item*, const std::string&)> onStringInputEvent;
    std::function<void(Item*, Point, PasteboardContent&)> onMouseDropEvent;
};

} // namespace uibot
