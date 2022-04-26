/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtItem.h"

#include <QMetaObject>
#include <QQuickItem>

#include <Scene/Qt/QtItemTools.h>

namespace spix {

QtItem::QtItem(QQuickItem* item)
: m_item(item)
{
}

Size QtItem::size() const
{
    return Size {m_item->width(), m_item->height()};
}

Point QtItem::position() const
{
    // the point (0, 0) in item coordinates...
    QPointF localPoint {0.0, 0.0};
    // ...is mapped to global to get the item position on screen
    auto globalPoint = m_item->mapToGlobal(localPoint);

    return Point {globalPoint.rx(), globalPoint.ry()};
}

Rect QtItem::bounds() const
{
    Rect bounds {0.0, 0.0, 0.0, 0.0};
    bounds.topLeft = position();
    bounds.size = size();
    return bounds;
}

std::string QtItem::stringProperty(const std::string& name) const
{
    auto value = m_item->property(name.c_str());
    return value.toString().toStdString();
}

void QtItem::setStringProperty(const std::string& name, const std::string& value)
{
    m_item->setProperty(name.c_str(), value.c_str());
}

bool QtItem::invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret)
{
    if (args.size() > 10)
        return false;

    std::vector<QVariant> qtVars;
    for (auto arg : args)
        qtVars.push_back(qt::VariantToQVariant(arg));

    QMetaMethod match;
    bool matched = spix::qt::GetMethodMetaForArgs(*m_item, method, qtVars, match);
    if (!matched)
        return false;

    qt::QMLReturnVariant retVar;
    QGenericReturnArgument retArg = qt::GetReturnArgForQMetaType(match.returnType(), retVar);
    std::vector<QGenericArgument> qtArgs = qt::ConvertAndCreateQArgumentsForMethod(match, qtVars);

    bool success = match.invoke(m_item, Qt::ConnectionType::DirectConnection, retArg, qtArgs[0], qtArgs[1], qtArgs[2],
        qtArgs[3], qtArgs[4], qtArgs[5], qtArgs[6], qtArgs[7], qtArgs[8], qtArgs[9]);
    if (success) {
        ret = qt::QMLReturnVariantToVariant(retVar);
        return true;
    }
    return false;
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
