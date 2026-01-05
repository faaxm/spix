/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/ItemPath.h>

class QWidget;

namespace spix {
namespace qt {

/**
 * Find a QWidget at the specified item path
 * @param path The item path to search for
 * @return The widget if found, nullptr otherwise
 */
QWidget* GetQWidgetAtPath(const spix::ItemPath& path);

/**
 * Find a top-level QWidget (window) at the specified item path.
 * Only the root element is used for the lookup.
 *
 * @param path The item path to search for
 * @return The QWidget if found, nullptr otherwise
 */
QWidget* GetTopLevelWidgetAtPath(const spix::ItemPath& path);

} // namespace qt
} // namespace spix
