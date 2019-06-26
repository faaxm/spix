/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/PasteboardContent.h>

namespace spix {

void PasteboardContent::addUrl(std::string url)
{
    m_urls.emplace_back(std::move(url));
}

bool PasteboardContent::hasUrls()
{
    return m_urls.size() > 0;
}

std::vector<std::string>& PasteboardContent::urls()
{
    return m_urls;
}

PasteboardContent makePasteboardContentWithUrls(const std::vector<std::string>& urls)
{
    PasteboardContent content;

    for (const auto& url : urls) {
        content.addUrl(url);
    }

    return content;
}

} // namespace uibot
