/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>
#include <vector>

namespace spix {
namespace utils {

/**
 * Parses a path string into its component parts, handling escaped characters.
 *
 * @param path The path string to parse (e.g., "window/item/subitem")
 * @return A vector of component strings with any escaped characters processed
 */
std::vector<std::string> ParsePathString(const std::string& path);

/**
 * Formats a vector of component strings into a properly escaped path string.
 *
 * @param components A vector of component strings
 * @return A formatted path string with proper escaping of special characters
 */
std::string FormatPathString(const std::vector<std::string>& components);

} // namespace utils
} // namespace spix
