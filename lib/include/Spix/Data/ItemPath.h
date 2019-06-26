/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>
#include <vector>

namespace spix {

class ItemPath {
public:
    ItemPath();
    ItemPath(const char* path);
    ItemPath(const std::string& path);
    ItemPath(std::vector<std::string> components);

    const std::vector<std::string>& components() const;
    size_t length() const { return m_components.size(); };
    std::string rootComponent() const;

    std::string string() const;

    ItemPath subPath(size_t offset) const;

private:
    std::vector<std::string> m_components;
};

} // namespace uibot
