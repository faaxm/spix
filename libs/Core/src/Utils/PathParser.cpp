/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Utils/PathParser.h>

namespace spix {
namespace utils {

std::vector<std::string> ParsePathString(const std::string& path)
{
    std::vector<std::string> components;
    std::string currentComponent;
    bool escapeNext = false;

    for (size_t i = 0; i < path.length(); ++i) {
        char c = path[i];

        if (escapeNext) {
            // If we're escaping this character, add it to the component regardless of what it is
            currentComponent += c;
            escapeNext = false;
        } else if (c == '\\') {
            // Backslash, set flag to escape the next character
            escapeNext = true;
        } else if (c == '/') {
            // Forward slash, save current component if not empty
            if (!currentComponent.empty()) {
                components.push_back(std::move(currentComponent));
                currentComponent.clear();
            }
        } else {
            // Regular character, add to current component
            currentComponent += c;
        }
    }

    // Add the last component if not empty
    if (!currentComponent.empty()) {
        components.push_back(std::move(currentComponent));
    }

    return components;
}

std::string FormatPathString(const std::vector<std::string>& components)
{
    std::string path;

    for (const auto& component : components) {
        if (!path.empty()) {
            path += '/';
        }

        // Escape any forward slashes and backslashes in the component value
        std::string escapedValue = component;

        // First, escape all backslashes by replacing \ with \\.
        size_t pos = 0;
        while ((pos = escapedValue.find('\\', pos)) != std::string::npos) {
            escapedValue.insert(pos, "\\");
            pos += 2; // Skip over the two backslashes
        }

        // Then, escape all forward slashes by replacing / with \/
        pos = 0;
        while ((pos = escapedValue.find('/', pos)) != std::string::npos) {
            escapedValue.insert(pos, "\\");
            pos += 2; // Skip over the backslash and forward slash
        }

        path += escapedValue;
    }

    return path;
}

} // namespace utils
} // namespace spix
