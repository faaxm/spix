/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtItem.h"

#include <QQuickItem>

namespace spix {

QtItem::QtItem(QQuickItem* item)
: m_item(item)
{
}

Size QtItem::size() const
{
    return Size {m_item->width(), m_item->height()};
}

std::string QtItem::stringProperty(const std::string& name) const
{
    auto value = m_item->property(name.c_str());
    return value.toString().toStdString();
}

bool QtItem::visible() const
{
    return m_item->isVisible();
}

QQuickItem* QtItem::qquickitem()
{
    return m_item;
}

} // namespace spix
