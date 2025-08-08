/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/ItemPath.h>
#include <Utils/PathParser.h>

#include <iterator>

namespace spix {

ItemPath::ItemPath() = default;

ItemPath::ItemPath(const char* path)
: ItemPath(std::string(path))
{
}

ItemPath::ItemPath(const std::string& path)
{
    auto stringComponents = utils::ParsePathString(path);
    for (const auto& component : stringComponents) {
        m_components.emplace_back(path::Component(component));
    }
}

ItemPath::ItemPath(std::vector<path::Component> components)
: m_components(std::move(components))
{
}

const std::vector<path::Component>& ItemPath::components() const
{
    return m_components;
}

const path::Component& ItemPath::rootComponent() const
{
    return m_components.at(0);
}

std::string ItemPath::string() const
{
    std::vector<std::string> stringComponents;
    for (const auto& component : m_components) {
        stringComponents.emplace_back(component.string());
    }
    return utils::FormatPathString(stringComponents);
}

ItemPath ItemPath::subPath(size_t offset) const
{
    if (offset >= m_components.size()) {
        return ItemPath();
    }

    std::vector<path::Component> sub_components;
    std::copy(m_components.begin() + offset, m_components.end(), std::back_inserter(sub_components));
    return ItemPath(std::move(sub_components));
}

} // namespace spix
