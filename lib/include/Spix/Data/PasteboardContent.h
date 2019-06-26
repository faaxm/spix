/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <string>
#include <vector>

namespace spix {

class PasteboardContent {
public:
    void addUrl(std::string url);
    bool hasUrls();
    std::vector<std::string>& urls();

private:
    std::vector<std::string> m_urls;
};

PasteboardContent makePasteboardContentWithUrls(const std::vector<std::string>& urls);

} // namespace spix
