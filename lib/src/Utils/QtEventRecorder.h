/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QObject>

namespace spix {
namespace utils {

class QtEventRecorder : public QObject {
public:
    bool eventFilter(QObject* object, QEvent* event);
};

} // namespace utils
} // namespace spix
