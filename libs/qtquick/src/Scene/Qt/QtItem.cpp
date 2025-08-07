/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtItem.h"

#include <QMetaObject>
#include <QQuickItem>
#include <QQuickWindow>

#include <Scene/Qt/QtItemTools.h>

namespace spix {

QtItem::QtItem(QQuickItem* item)
: m_item(item)
{
}

QtItem::QtItem(QQuickWindow* window)
: m_item(window)
{
}

Size QtItem::size() const
{
    return Size {qquickitem()->width(), qquickitem()->height()};
}

Point QtItem::position() const
{
    // the point (0, 0) in item coordinates...
    QPointF localPoint {0.0, 0.0};
    // ...is mapped to global to get the item position on screen
    auto globalPoint = qquickitem()->mapToGlobal(localPoint);

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
    auto value = qobject()->property(name.c_str());
    return value.toString().toStdString();
}

void QtItem::setStringProperty(const std::string& name, const std::string& value)
{
    qobject()->setProperty(name.c_str(), value.c_str());
}

bool QtItem::invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret)
{
    if (args.size() > 10)
        return false;

    std::vector<QVariant> qtVars;
    for (auto arg : args)
        qtVars.push_back(qt::VariantToQVariant(arg));

    QMetaMethod match;
    bool matched = spix::qt::GetMethodMetaForArgs(*qobject(), method, qtVars, match);
    if (!matched)
        return false;

    qt::QMLReturnVariant retVar;
    QGenericReturnArgument retArg = qt::GetReturnArgForQMetaType(match.returnType(), retVar);
    std::vector<QGenericArgument> qtArgs = qt::ConvertAndCreateQArgumentsForMethod(match, qtVars);

    bool success = match.invoke(qobject(), Qt::ConnectionType::DirectConnection, retArg, qtArgs[0], qtArgs[1],
        qtArgs[2], qtArgs[3], qtArgs[4], qtArgs[5], qtArgs[6], qtArgs[7], qtArgs[8], qtArgs[9]);
    if (success) {
        ret = qt::QMLReturnVariantToVariant(retVar);
        return true;
    }
    return false;
}

bool QtItem::visible() const
{
    return qquickitem()->isVisible();
}

QQuickItem* QtItem::qquickitem()
{
    if (std::holds_alternative<QQuickWindow*>(m_item))
        return std::get<QQuickWindow*>(m_item)->contentItem();
    else
        return std::get<QQuickItem*>(m_item);
}

const QQuickItem* QtItem::qquickitem() const
{
    if (std::holds_alternative<QQuickWindow*>(m_item))
        return std::get<QQuickWindow*>(m_item)->contentItem();
    else
        return std::get<QQuickItem*>(m_item);
}

QObject* QtItem::qobject()
{
    return std::visit([](auto i) { return static_cast<QObject*>(i); }, m_item);
}

const QObject* QtItem::qobject() const
{
    return std::visit([](auto i) { return static_cast<const QObject*>(i); }, m_item);
}

} // namespace spix
