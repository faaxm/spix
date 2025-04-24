/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>
#include <variant>
#include <Spix/spix_export.h>

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

using Selector = std::variant<NameSelector, PropertySelector>;

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