/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include <string>
#include <variant>

namespace spix {
namespace path {

/**
 * @brief Selector for item path by name
 */
class SPIX_EXPORT NameSelector {
public:
    NameSelector() = default;
    explicit NameSelector(std::string name);

    const std::string& name() const;

private:
    std::string m_name;
};

/**
 * @brief Selector for item path by property (starting with '.')
 */
class SPIX_EXPORT PropertySelector {
public:
    PropertySelector() = default;
    explicit PropertySelector(std::string name);

    const std::string& name() const;

private:
    std::string m_name;
};

/**
 * @brief Selector for item path by type
 */
class SPIX_EXPORT TypeSelector {
public:
    TypeSelector() = default;
    explicit TypeSelector(std::string type);

    const std::string& type() const;

private:
    std::string m_type;
};

/**
 * @brief Selector for item path by text property value (surrounded by quotes '"')
 */
class SPIX_EXPORT ValueSelector {
public:
    ValueSelector() = default;
    explicit ValueSelector(std::string value);

    const std::string& value() const;

private:
    std::string m_value;
};

/**
 * @brief Selector for item path by property name and value (format: "(propertyName=propertyValue)")
 */
class SPIX_EXPORT PropertyValueSelector {
public:
    PropertyValueSelector() = default;
    PropertyValueSelector(std::string propertyName, std::string propertyValue);

    const std::string& propertyName() const;
    const std::string& propertyValue() const;

private:
    std::string m_propertyName;
    std::string m_propertyValue;
};

using Selector = std::variant<NameSelector, PropertySelector, TypeSelector, ValueSelector, PropertyValueSelector>;

/**
 * @brief A component of an item path
 */
class SPIX_EXPORT Component {
public:
    Component() = default;
    explicit Component(const std::string& rawValue);
    explicit Component(Selector selector);

    std::string string() const;
    const Selector& selector() const;

private:
    Selector m_selector;
};

} // namespace path
} // namespace spix
