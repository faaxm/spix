/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Spix/Data/ItemPosition.h>

TEST(ItemPositionTest, ApplyToSize)
{
    spix::ItemPosition itemPos {"windowname/item/subitem", {1.0, 0.0}, {10.0, -5.0}};
    spix::Size itemSize {100.0, 40.0};

    auto resolvedPos = itemPos.positionForItemSize(itemSize);

    EXPECT_DOUBLE_EQ(resolvedPos.x, 110.0);
    EXPECT_DOUBLE_EQ(resolvedPos.y, -5.0);
}
