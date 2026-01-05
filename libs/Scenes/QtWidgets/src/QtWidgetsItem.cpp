/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtWidgetsItem.h"

#include <QMetaMethod>
#include <QMetaObject>
#include <QWidget>

#include <QtWidgetsItemTools.h>

namespace spix {

QtWidgetsItem::QtWidgetsItem(QWidget* widget)
: m_widget(widget)
{
}

Size QtWidgetsItem::size() const
{
    QSize s = m_widget->size();
    return Size {static_cast<double>(s.width()), static_cast<double>(s.height())};
}

Point QtWidgetsItem::position() const
{
    // the point (0, 0) in widget coordinates...
    QPoint localPoint {0, 0};
    // ...is mapped to global to get the widget position on screen
    auto globalPoint = m_widget->mapToGlobal(localPoint);

    return Point {static_cast<double>(globalPoint.x()), static_cast<double>(globalPoint.y())};
}

Rect QtWidgetsItem::bounds() const
{
    Rect bounds {0.0, 0.0, 0.0, 0.0};
    bounds.topLeft = position();
    bounds.size = size();
    return bounds;
}

std::string QtWidgetsItem::stringProperty(const std::string& name) const
{
    auto value = qobject()->property(name.c_str());
    return value.toString().toStdString();
}

void QtWidgetsItem::setStringProperty(const std::string& name, const std::string& value)
{
    qobject()->setProperty(name.c_str(), value.c_str());
}

bool QtWidgetsItem::invokeMethod(const std::string& method, const std::vector<Variant>& args, Variant& ret)
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

bool QtWidgetsItem::visible() const
{
    return m_widget->isVisible();
}

QWidget* QtWidgetsItem::qwidget()
{
    return m_widget;
}

const QWidget* QtWidgetsItem::qwidget() const
{
    return m_widget;
}

QObject* QtWidgetsItem::qobject()
{
    return static_cast<QObject*>(m_widget);
}

const QObject* QtWidgetsItem::qobject() const
{
    return static_cast<const QObject*>(m_widget);
}

} // namespace spix
