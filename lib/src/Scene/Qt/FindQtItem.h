/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/ItemPath.h>
#include <QQuickItem>
#include <QQuickWindow>

namespace spix {
namespace qt {

/**
 * Find a QQuickItem at the specified item path
 * @param path The item path to search for
 * @return The item if found, nullptr otherwise
 */
QQuickItem* GetQQuickItemAtPath(const spix::ItemPath& path);

} // namespace qt
} // namespace spix 