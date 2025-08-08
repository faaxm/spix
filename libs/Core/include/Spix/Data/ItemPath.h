/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>
#include <vector>

#include <Spix/Data/ItemPathComponent.h>
#include <Spix/spix_core_export.h>

namespace spix {

/**
 * @brief A path to locate an item in the scene
 *
 * The item path is a list of item names, separated by slashes '/' with the
 * first item usually identifying the window.
 * A path like 'window/item1/subitem2' would reference the 'subitem2' which is
 * a child of 'item1' in the window 'window'.
 *
 * By default, the search for an item by its name is recursive.
 * If an item is not found at the root level of the window, the children
 * of each item in the root level will be searched.
 * This means that not all items along the path have to be named.
 *
 * In QML, items can be named by setting the 'objectName' string property.
 */
class SPIXCORE_EXPORT ItemPath {
public:
    ItemPath();
    ItemPath(const char* path);
    ItemPath(const std::string& path);
    ItemPath(std::vector<path::Component> components);

    const std::vector<path::Component>& components() const;
    size_t length() const { return m_components.size(); };
    const path::Component& rootComponent() const;

    std::string string() const;

    ItemPath subPath(size_t offset) const;

private:
    std::vector<path::Component> m_components;
};

} // namespace spix
