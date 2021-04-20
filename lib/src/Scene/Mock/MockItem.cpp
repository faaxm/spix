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

Point MockItem::position() const
{
    return Point {0.0, 0.0};
}

Rect MockItem::bounds() const
{
    Rect bounds {0.0, 0.0, 0.0, 0.0};
    bounds.topLeft = position();
    bounds.size = size();
    return bounds;
}

std::string MockItem::stringProperty(const std::string& name) const
{
    return m_stringProperties.at(name);
}

void MockItem::setStringProperty(const std::string& name, const std::string& value)
{
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
