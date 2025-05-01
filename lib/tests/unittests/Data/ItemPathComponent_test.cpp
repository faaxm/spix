/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Spix/Data/ItemPathComponent.h>
#include <variant>

TEST(ItemPathComponentTest, Construction)
{
    // Test regular name component
    spix::path::Component nameComp("test");
    EXPECT_EQ(nameComp.string(), "test");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(nameComp.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(nameComp.selector()).name(), "test");

    // Test property component
    spix::path::Component propertyComp(".testProperty");
    EXPECT_EQ(propertyComp.string(), ".testProperty");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertySelector>(propertyComp.selector()));
    EXPECT_EQ(std::get<spix::path::PropertySelector>(propertyComp.selector()).name(), "testProperty");

    // Test empty component
    spix::path::Component emptyComp("");
    EXPECT_EQ(emptyComp.string(), "");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(emptyComp.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(emptyComp.selector()).name(), "");

    // Test construction from name selector
    spix::path::NameSelector nameSelector("selectorName");
    spix::path::Component fromNameSelector(nameSelector);
    EXPECT_EQ(fromNameSelector.string(), "selectorName");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(fromNameSelector.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(fromNameSelector.selector()).name(), "selectorName");

    // Test construction from property selector
    spix::path::PropertySelector propSelector("propertyName");
    spix::path::Component fromPropSelector(propSelector);
    EXPECT_EQ(fromPropSelector.string(), ".propertyName");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertySelector>(fromPropSelector.selector()));
    EXPECT_EQ(std::get<spix::path::PropertySelector>(fromPropSelector.selector()).name(), "propertyName");
}

TEST(ItemPathComponentTest, Selectors)
{
    // Test ItemPathNameSelector
    spix::path::NameSelector nameSelector("name");
    EXPECT_EQ(nameSelector.name(), "name");

    // Test ItemPathPropertySelector
    spix::path::PropertySelector propertySelector("property");
    EXPECT_EQ(propertySelector.name(), "property");
}