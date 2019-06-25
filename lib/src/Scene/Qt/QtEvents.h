/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Scene/Events.h>

namespace uibot {

class QtEvents : public Events {
public:
    void mouseClick(Item* item, Point loc, bool press = true, bool release = true) override;
    void stringInput(Item* item, const std::string& text) override;
    void extMouseDrop(Item* item, Point loc, PasteboardContent& content) override;
    void quit() override;
};

} // namespace uibot
