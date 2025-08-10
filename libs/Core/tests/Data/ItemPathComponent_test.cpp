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

    // Test type component
    spix::path::Component typeComp("#Button");
    EXPECT_EQ(typeComp.string(), "#Button");
    EXPECT_TRUE(std::holds_alternative<spix::path::TypeSelector>(typeComp.selector()));
    EXPECT_EQ(std::get<spix::path::TypeSelector>(typeComp.selector()).type(), "Button");

    // Test value component
    spix::path::Component valueComp("\"Hello World\"");
    EXPECT_EQ(valueComp.string(), "\"Hello World\"");
    EXPECT_TRUE(std::holds_alternative<spix::path::ValueSelector>(valueComp.selector()));
    EXPECT_EQ(std::get<spix::path::ValueSelector>(valueComp.selector()).value(), "Hello World");

    // Test property value component
    spix::path::Component propValueComp("(text=Submit)");
    EXPECT_EQ(propValueComp.string(), "(text=Submit)");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertyValueSelector>(propValueComp.selector()));
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(propValueComp.selector()).propertyName(), "text");
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(propValueComp.selector()).propertyValue(), "Submit");

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

    // Test construction from type selector
    spix::path::TypeSelector typeSelector("Button");
    spix::path::Component fromTypeSelector(typeSelector);
    EXPECT_EQ(fromTypeSelector.string(), "#Button");
    EXPECT_TRUE(std::holds_alternative<spix::path::TypeSelector>(fromTypeSelector.selector()));
    EXPECT_EQ(std::get<spix::path::TypeSelector>(fromTypeSelector.selector()).type(), "Button");

    // Test construction from value selector
    spix::path::ValueSelector valueSelector("Click me");
    spix::path::Component fromValueSelector(valueSelector);
    EXPECT_EQ(fromValueSelector.string(), "\"Click me\"");
    EXPECT_TRUE(std::holds_alternative<spix::path::ValueSelector>(fromValueSelector.selector()));
    EXPECT_EQ(std::get<spix::path::ValueSelector>(fromValueSelector.selector()).value(), "Click me");

    // Test construction from property value selector
    spix::path::PropertyValueSelector propValueSelector("enabled", "true");
    spix::path::Component fromPropValueSelector(propValueSelector);
    EXPECT_EQ(fromPropValueSelector.string(), "(enabled=true)");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertyValueSelector>(fromPropValueSelector.selector()));
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(fromPropValueSelector.selector()).propertyName(), "enabled");
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(fromPropValueSelector.selector()).propertyValue(), "true");
}

TEST(ItemPathComponentTest, Selectors)
{
    // Test ItemPathNameSelector
    spix::path::NameSelector nameSelector("name");
    EXPECT_EQ(nameSelector.name(), "name");

    // Test ItemPathPropertySelector
    spix::path::PropertySelector propertySelector("property");
    EXPECT_EQ(propertySelector.name(), "property");

    // Test ItemPathTypeSelector
    spix::path::TypeSelector typeSelector("Button");
    EXPECT_EQ(typeSelector.type(), "Button");

    // Test ItemPathValueSelector
    spix::path::ValueSelector valueSelector("Hello World");
    EXPECT_EQ(valueSelector.value(), "Hello World");

    // Test ItemPathPropertyValueSelector
    spix::path::PropertyValueSelector propValueSelector("visible", "false");
    EXPECT_EQ(propValueSelector.propertyName(), "visible");
    EXPECT_EQ(propValueSelector.propertyValue(), "false");
}

// Add additional tests for edge cases
TEST(ItemPathComponentTest, EdgeCases)
{
    // Test empty value selectors
    spix::path::Component emptyValueComp("\"\"");
    EXPECT_TRUE(std::holds_alternative<spix::path::ValueSelector>(emptyValueComp.selector()));
    EXPECT_EQ(std::get<spix::path::ValueSelector>(emptyValueComp.selector()).value(), "");

    // Test property value selector with empty property name
    spix::path::Component emptyPropNameComp("(=value)");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertyValueSelector>(emptyPropNameComp.selector()));
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(emptyPropNameComp.selector()).propertyName(), "");
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(emptyPropNameComp.selector()).propertyValue(), "value");

    // Test property value selector with empty property value
    spix::path::Component emptyPropValueComp("(key=)");
    EXPECT_TRUE(std::holds_alternative<spix::path::PropertyValueSelector>(emptyPropValueComp.selector()));
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(emptyPropValueComp.selector()).propertyName(), "key");
    EXPECT_EQ(std::get<spix::path::PropertyValueSelector>(emptyPropValueComp.selector()).propertyValue(), "");

    // Test property value selector without equals sign - should be treated as name selector
    spix::path::Component noEqualsComp("(noEquals)");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(noEqualsComp.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(noEqualsComp.selector()).name(), "(noEquals)");

    // Test malformed selectors
    spix::path::Component unfinishedValueComp("\"Unfinished");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(unfinishedValueComp.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(unfinishedValueComp.selector()).name(), "\"Unfinished");

    spix::path::Component unfinishedPropValueComp("(text=value");
    EXPECT_TRUE(std::holds_alternative<spix::path::NameSelector>(unfinishedPropValueComp.selector()));
    EXPECT_EQ(std::get<spix::path::NameSelector>(unfinishedPropValueComp.selector()).name(), "(text=value");
}
