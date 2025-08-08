/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtEventRecorder.h"

#include <QEvent>
#include <QMouseEvent>
#include <iostream>

namespace spix {
namespace utils {

bool QtEventRecorder::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        std::cout << "Button: " << mouseEvent->button() << "  Buttons: " << mouseEvent->buttons() << std::endl;
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        std::cout << "Button: " << mouseEvent->button() << "  Buttons: " << mouseEvent->buttons() << std::endl;
    }
    return false;
}

} // namespace utils
} // namespace spix
