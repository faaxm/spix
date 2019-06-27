/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Geometry.h>
#include <Spix/Data/ItemPath.h>
#include <string>

namespace spix {

/**
 * @brief Position in the scene, relative to an item
 */
class ItemPosition {
public:
    /**
     * @brief Construct an ItemPosition with a proportional and a fixed position offset
     *
     * To describe a position that is 10 pixels to the left and 5 pixels below the upper
     * left corner of an item, construct:
     * `ItemPosition("window/some/item", {0.0, 0.0}, {-10.0, 5.0});`
     *
     * @param path         Path to the item
     * @param proportion   Proportional position in the item. {.5, .5} is
     *                     the item's center.
     * @param offset       Offset applied to the proportional position.
     */
    ItemPosition(ItemPath path, Point proportion = {0.5, 0.5}, Point offset = {0., 0.});

    const ItemPath& itemPath() const;
    Point positionForItemSize(const Size& size) const;

private:
    ItemPath m_itemPath;
    Point m_proportion;
    Point m_offset;
};

} // namespace spix
