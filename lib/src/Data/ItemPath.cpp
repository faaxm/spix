/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/ItemPath.h>

#include <iterator>
#include <sstream>

namespace spix {

ItemPath::ItemPath() = default;

ItemPath::ItemPath(const char* path)
: ItemPath(std::string(path))
{
}

ItemPath::ItemPath(const std::string& path)
{
    std::stringstream pathss(path);

    while (pathss) {
        std::string component;
        if (pathss.peek() == '\"') {
            getline(pathss, component, '\"');
            getline(pathss, component, '\"');

            if (pathss.peek() == '#') {
                std::string number = "";
                getline(pathss, number, '#');

                char nextChar;
                while (std::isdigit(pathss.peek())) {
                    pathss >> nextChar;
                    number += nextChar;
                }
                component = '\"' + component + '\"' + '#' + number;
            } else {
                component = '\"' + component + '\"';
            }
            m_components.push_back(std::move(component));
        }

        if (pathss.peek() == '(') {
            getline(pathss, component, '(');
            getline(pathss, component, ')');

            if (pathss.peek() == '#') {
                std::string number = "";
                getline(pathss, number, '#');

                char nextChar;
                while (std::isdigit(pathss.peek())) {
                    pathss >> nextChar;
                    number += nextChar;
                }
                component = '(' + component + ')' + '#' + number;
            } else {
                component = '(' + component + ')';
            }
            m_components.push_back(std::move(component));
        }

        getline(pathss, component, '/');
        if (component.length() > 0) {
            m_components.push_back(std::move(component));
        }
    }
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
    std::string path;

    for (auto& component : m_components) {
        if (!path.empty()) {
            path += "/";
        }
        path += component;
    }

    return path;
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
