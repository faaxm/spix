/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Utils/PathParser.h>
#include <string>
#include <vector>

TEST(PathParserTest, ParsePathString_Basic)
{
    // Basic path parsing
    std::vector<std::string> components = spix::utils::ParsePathString("window/item/subitem");

    EXPECT_EQ(components.size(), 3);
    EXPECT_EQ(components.at(0), "window");
    EXPECT_EQ(components.at(1), "item");
    EXPECT_EQ(components.at(2), "subitem");
}

TEST(PathParserTest, ParsePathString_Empty)
{
    // Empty path
    std::vector<std::string> empty_components = spix::utils::ParsePathString("");
    EXPECT_EQ(empty_components.size(), 0);

    // Path with just slashes
    std::vector<std::string> slash_components = spix::utils::ParsePathString("///");
    EXPECT_EQ(slash_components.size(), 0);
}

TEST(PathParserTest, ParsePathString_ExtraSlashes)
{
    // Path with extra slashes
    std::vector<std::string> components = spix::utils::ParsePathString("/windowname/item/subitem/");

    EXPECT_EQ(components.size(), 3);
    EXPECT_EQ(components.at(0), "windowname");
    EXPECT_EQ(components.at(1), "item");
    EXPECT_EQ(components.at(2), "subitem");
}

TEST(PathParserTest, ParsePathString_EscapedSlashes)
{
    // Test path with escaped slashes
    std::vector<std::string> components = spix::utils::ParsePathString("window\\/name/item/sub\\/item");

    EXPECT_EQ(components.size(), 3);
    EXPECT_EQ(components.at(0), "window/name");
    EXPECT_EQ(components.at(1), "item");
    EXPECT_EQ(components.at(2), "sub/item");

    // Path with only escaped content
    std::vector<std::string> escaped_only = spix::utils::ParsePathString("\\/\\/");
    EXPECT_EQ(escaped_only.size(), 1);
    EXPECT_EQ(escaped_only.at(0), "//");

    // Complex path with multiple escaped slashes
    std::vector<std::string> complex = spix::utils::ParsePathString("a\\/b\\/c/d/e\\/f\\/g");
    EXPECT_EQ(complex.size(), 3);
    EXPECT_EQ(complex.at(0), "a/b/c");
    EXPECT_EQ(complex.at(1), "d");
    EXPECT_EQ(complex.at(2), "e/f/g");
}

TEST(PathParserTest, ParsePathString_Backslashes)
{
    // Test how backslashes are handled

    // 1. A single backslash that escapes a normal character (should remove the backslash)
    std::vector<std::string> components1 = spix::utils::ParsePathString("window\\name/item");
    EXPECT_EQ(components1.size(), 2);
    EXPECT_EQ(components1.at(0), "windowname"); // Backslash should be removed
    EXPECT_EQ(components1.at(1), "item");

    // 2. A double backslash (escaping a backslash)
    std::vector<std::string> components2 = spix::utils::ParsePathString("window\\\\name/item");
    EXPECT_EQ(components2.size(), 2);
    EXPECT_EQ(components2.at(0), "window\\name"); // Should contain one backslash
    EXPECT_EQ(components2.at(1), "item");

    // 3. A backslash at the end of a component (should be preserved)
    std::vector<std::string> components3 = spix::utils::ParsePathString("window\\/item\\\\/"); // "window/item\"/"
    EXPECT_EQ(components3.size(), 1);
    EXPECT_EQ(components3.at(0), "window/item\\"); // Should contain a slash and a backslash

    // 4. A trailing backslash in the path string
    std::vector<std::string> components4 = spix::utils::ParsePathString("component\\");
    EXPECT_EQ(components4.size(), 1);
    EXPECT_EQ(components4.at(0), "component");
}

TEST(PathParserTest, FormatPathString_Basic)
{
    // Basic path formatting
    std::vector<std::string> components = {"window", "item", "subitem"};
    std::string path = spix::utils::FormatPathString(components);

    EXPECT_EQ(path, "window/item/subitem");
}

TEST(PathParserTest, FormatPathString_Empty)
{
    // Empty components
    std::vector<std::string> empty_components;
    std::string empty_path = spix::utils::FormatPathString(empty_components);

    EXPECT_EQ(empty_path, "");

    // Single empty component
    std::vector<std::string> single_empty = {""};
    std::string single_empty_path = spix::utils::FormatPathString(single_empty);

    EXPECT_EQ(single_empty_path, "");
}

TEST(PathParserTest, FormatPathString_EscapedSlashes)
{
    // Components with slashes that need escaping
    std::vector<std::string> components = {"window/name", "item", "sub/item"};
    std::string path = spix::utils::FormatPathString(components);

    EXPECT_EQ(path, "window\\/name/item/sub\\/item");
}

TEST(PathParserTest, FormatPathString_Backslashes)
{
    // Components with backslashes that need escaping
    std::vector<std::string> components = {"back\\slash", "item", "sub\\item"};
    std::string path = spix::utils::FormatPathString(components);

    EXPECT_EQ(path, "back\\\\slash/item/sub\\\\item");

    // Complex component with both backslashes and forward slashes
    std::vector<std::string> complex = {"a/b\\c", "d", "e\\f/g"};
    std::string complex_path = spix::utils::FormatPathString(complex);

    EXPECT_EQ(complex_path, "a\\/b\\\\c/d/e\\\\f\\/g");
}

TEST(PathParserTest, RoundTrip)
{
    // Test that parsing and then formatting produces the same result

    // 1. Simple path
    std::string original1 = "window/item/subitem";
    std::vector<std::string> components1 = spix::utils::ParsePathString(original1);
    std::string formatted1 = spix::utils::FormatPathString(components1);
    EXPECT_EQ(formatted1, original1);

    // 2. Path with escaped characters
    std::string original2 = "window\\/name/item\\\\name/sub\\/item";
    std::vector<std::string> components2 = spix::utils::ParsePathString(original2);
    std::string formatted2 = spix::utils::FormatPathString(components2);
    EXPECT_EQ(formatted2, original2);

    // 3. Idempotence: formatting then parsing then formatting again
    std::vector<std::string> original_components = {"window/item", "sub\\item"};
    std::string path = spix::utils::FormatPathString(original_components);
    std::vector<std::string> parsed_components = spix::utils::ParsePathString(path);
    std::string reforced_path = spix::utils::FormatPathString(parsed_components);

    EXPECT_EQ(parsed_components, original_components);
    EXPECT_EQ(reforced_path, path);
}
