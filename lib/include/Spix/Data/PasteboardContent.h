/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "spix_export.h"

#include <string>
#include <vector>

namespace spix {

/**
 * @brief PasteboardContent represent data from external apps
 *
 * This data can be used to represent MIME data that is dropped onto the
 * application from external applications.
 */
class SPIX_EXPORT PasteboardContent {
public:
    void addUrl(std::string url);
    bool hasUrls();
    std::vector<std::string>& urls();

private:
    std::vector<std::string> m_urls;
};

PasteboardContent SPIX_EXPORT makePasteboardContentWithUrls(const std::vector<std::string>& urls);

} // namespace spix
