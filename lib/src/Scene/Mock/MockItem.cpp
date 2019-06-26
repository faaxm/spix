/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "MockItem.h"

namespace spix {

MockItem::MockItem(Size size)
: m_size(size)
{
}

Size MockItem::size() const
{
    return m_size;
}

std::string MockItem::stringProperty(const std::string& name) const
{
    return m_stringProperties.at(name);
}

bool MockItem::visible() const
{
    return true;
}

std::map<std::string, std::string>& MockItem::stringProperties()
{
    return m_stringProperties;
}

} // namespace spix
