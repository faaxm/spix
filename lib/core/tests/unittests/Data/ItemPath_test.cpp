/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Spix/Data/ItemPath.h>

TEST(ItemPathTest, InitWithPathString)
{
    spix::ItemPath path {"windowname/item/subitem"};
    auto components = path.components();

    EXPECT_EQ(components.size(), 3);
    EXPECT_EQ(components.at(0), "windowname");
    EXPECT_EQ(components.at(1), "item");
    EXPECT_EQ(components.at(2), "subitem");

    spix::ItemPath empty_path {""};
    auto empty_comp = empty_path.components();
    EXPECT_EQ(empty_comp.size(), 0);

    spix::ItemPath more_slashes_path {"/windowname/item/subitem/"};
    auto more_slashes_comp = more_slashes_path.components();
    EXPECT_EQ(more_slashes_comp.size(), 3);
    EXPECT_EQ(more_slashes_comp.at(0), "windowname");
    EXPECT_EQ(more_slashes_comp.at(1), "item");
    EXPECT_EQ(more_slashes_comp.at(2), "subitem");
}

TEST(ItemPathTest, InitWithComponents)
{
    std::vector<std::string> in_components {"windowname", "item", "subitem"};
    spix::ItemPath path(in_components);
    auto out_components = path.components();

    EXPECT_EQ(out_components.size(), 3);
    EXPECT_EQ(out_components.at(0), "windowname");
    EXPECT_EQ(out_components.at(1), "item");
    EXPECT_EQ(out_components.at(2), "subitem");
}

TEST(ItemPathTest, SubPath)
{
    spix::ItemPath path {"windowname/item/subitem/subsubitem"};

    auto subPath_1 = path.subPath(0);
    EXPECT_EQ(subPath_1.string(), "windowname/item/subitem/subsubitem");

    auto subPath_2 = path.subPath(1);
    EXPECT_EQ(subPath_2.string(), "item/subitem/subsubitem");

    auto subPath_3 = path.subPath(3);
    EXPECT_EQ(subPath_3.string(), "subsubitem");

    auto subPath_4 = path.subPath(4);
    EXPECT_EQ(subPath_4.string(), "");

    auto subPath_100 = path.subPath(100);
    EXPECT_EQ(subPath_100.string(), "");
}
