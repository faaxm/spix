/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "MockEvents.h"

namespace uibot {

void MockEvents::mouseClick(Item* item, Point loc, bool press, bool release)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, press, release);
    }
}

void MockEvents::stringInput(Item* item, const std::string& text)
{
    if (onStringInputEvent) {
        onStringInputEvent(item, text);
    }
}

void MockEvents::extMouseDrop(Item* item, Point loc, PasteboardContent& content)
{
    if (onMouseDropEvent) {
        onMouseDropEvent(item, loc, content);
    }
}

void MockEvents::quit()
{
}

} // namespace uibot
