/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "MockEvents.h"

namespace spix {

void MockEvents::mouseDown(Item* item, Point loc, MouseButton button)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, true, false);
    }
}

void MockEvents::mouseUp(Item* item, Point loc, MouseButton button)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, false, true);
    }
}

void MockEvents::mouseMove(Item* item, Point loc)
{
    if (onMouseClickEvent) {
        onMouseClickEvent(item, loc, false, false);
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
