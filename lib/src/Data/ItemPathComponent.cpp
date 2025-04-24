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

// Component implementation
Component::Component(const std::string& rawValue)
{
    // If the raw value starts with '.', create a property selector
    if (!rawValue.empty() && rawValue[0] == '.') {
        std::string propertyName = rawValue.substr(1); // Remove the leading '.'
        m_selector = PropertySelector(propertyName);
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
    }
    return "";
}

const Selector& Component::selector() const
{
    return m_selector;
}

} // namespace path
} // namespace spix 