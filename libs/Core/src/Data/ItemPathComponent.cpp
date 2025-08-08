/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/ItemPathComponent.h>
#include <variant>

namespace spix {
namespace path {

// NameSelector implementation
NameSelector::NameSelector(std::string name)
: m_name(std::move(name))
{
}

const std::string& NameSelector::name() const
{
    return m_name;
}

// PropertySelector implementation
PropertySelector::PropertySelector(std::string name)
: m_name(std::move(name))
{
}

const std::string& PropertySelector::name() const
{
    return m_name;
}

// TypeSelector implementation
TypeSelector::TypeSelector(std::string type)
: m_type(std::move(type))
{
}

const std::string& TypeSelector::type() const
{
    return m_type;
}

// ValueSelector implementation
ValueSelector::ValueSelector(std::string value)
: m_value(std::move(value))
{
}

const std::string& ValueSelector::value() const
{
    return m_value;
}

// PropertyValueSelector implementation
PropertyValueSelector::PropertyValueSelector(std::string propertyName, std::string propertyValue)
: m_propertyName(std::move(propertyName))
, m_propertyValue(std::move(propertyValue))
{
}

const std::string& PropertyValueSelector::propertyName() const
{
    return m_propertyName;
}

const std::string& PropertyValueSelector::propertyValue() const
{
    return m_propertyValue;
}

// Component implementation
Component::Component(const std::string& rawValue)
{
    // If the raw value starts with '.', create a property selector
    if (!rawValue.empty() && rawValue[0] == '.') {
        std::string propertyName = rawValue.substr(1); // Remove the leading '.'
        m_selector = PropertySelector(propertyName);
    }
    // If the raw value starts with '#', create a type selector
    else if (!rawValue.empty() && rawValue[0] == '#') {
        std::string typeName = rawValue.substr(1); // Remove the leading '#'
        m_selector = TypeSelector(typeName);
    }
    // If the raw value starts with '"' and ends with '"', create a value selector
    else if (rawValue.size() >= 2 && rawValue[0] == '"' && rawValue[rawValue.size() - 1] == '"') {
        std::string value = rawValue.substr(1, rawValue.size() - 2); // Remove the quotes
        m_selector = ValueSelector(value);
    }
    // If the raw value starts with '(' and ends with ')', create a property value selector
    else if (rawValue.size() >= 2 && rawValue[0] == '(' && rawValue[rawValue.size() - 1] == ')') {
        std::string content = rawValue.substr(1, rawValue.size() - 2); // Remove the parentheses

        // Find the equals sign separating property name and value
        size_t equalsPos = content.find('=');
        if (equalsPos != std::string::npos) {
            std::string propName = content.substr(0, equalsPos);
            std::string propValue = content.substr(equalsPos + 1);
            m_selector = PropertyValueSelector(propName, propValue);
        } else {
            // If no equals sign found, fall back to name selector
            m_selector = NameSelector(rawValue);
        }
    } else {
        m_selector = NameSelector(rawValue);
    }
}

Component::Component(Selector selector)
: m_selector(std::move(selector))
{
}

std::string Component::string() const
{
    if (std::holds_alternative<NameSelector>(m_selector)) {
        return std::get<NameSelector>(m_selector).name();
    } else if (std::holds_alternative<PropertySelector>(m_selector)) {
        return "." + std::get<PropertySelector>(m_selector).name();
    } else if (std::holds_alternative<TypeSelector>(m_selector)) {
        return "#" + std::get<TypeSelector>(m_selector).type();
    } else if (std::holds_alternative<ValueSelector>(m_selector)) {
        return "\"" + std::get<ValueSelector>(m_selector).value() + "\"";
    } else if (std::holds_alternative<PropertyValueSelector>(m_selector)) {
        const auto& propValSelector = std::get<PropertyValueSelector>(m_selector);
        return "(" + propValSelector.propertyName() + "=" + propValSelector.propertyValue() + ")";
    }
    return "";
}

const Selector& Component::selector() const
{
    return m_selector;
}

} // namespace path
} // namespace spix
