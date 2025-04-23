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
: m_components(utils::ParsePathString(path))
{
}

ItemPath::ItemPath(std::vector<std::string> components)
: m_components(std::move(components))
{
}

const std::vector<std::string>& ItemPath::components() const
{
    return m_components;
}

std::string ItemPath::rootComponent() const
{
    return m_components.at(0);
}

std::string ItemPath::string() const
{
    return utils::FormatPathString(m_components);
}

ItemPath ItemPath::subPath(size_t offset) const
{
    if (offset >= m_components.size()) {
        return ItemPath();
    }

    std::vector<std::string> sub_components;
    std::copy(m_components.begin() + offset, m_components.end(), std::back_inserter(sub_components));
    return ItemPath(std::move(sub_components));
}

} // namespace spix
