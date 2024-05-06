/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Variant.h>

#include <QDateTime>
#include <QObject>
#include <QQuickItem>
#include <QVariant>
#include <optional>

class QString;

namespace spix {
namespace qt {

extern const QString repeater_class_name;
extern const char* item_at_method_name;

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index);
QQuickItem* RepeaterChildWithName(QQuickItem* repeater, const QString& name);

QString GetObjectName(QObject* object);
QString PropertValueByObject(QObject* object, QString propertyName);
QString TypeByObject(QObject* object);

/**
 * @brief Find a child object with the given name.
 *
 * This works similar to `QObject::findChild`. However, once it
 * encounters a `QQuickItem`, it no longer iterates over the object's
 * `children()`, but rather its `childItems()`.
 */

QObject* FindChildItemByName(QObject* object, const QString& name, int matchIndex = 1);
template <typename T>
T FindChildItemByName(QObject* object, const QString& name, int matchIndex = 1)
{
    return qobject_cast<T>(FindChildItemByName(object, name, matchIndex));
}

QObject* FindChildItemByProperty(
    QObject* object, const QString& propertyName, const QString& propertyValue, int matchIndex = 1);
template <typename T>
T FindChildItemByProperty(
    QObject* object, const QString& propertyName, const QString& propertyValue, int matchIndex = 1)
{
    return qobject_cast<T>(FindChildItemByProperty(object, propertyName, propertyValue, matchIndex));
}

QVector<QObject*> FindChildItemsByType(QObject* object, const QString& type, int matchIndex);

using QMLReturnVariant = std::variant<std::nullptr_t, bool, int, float, double, QString, QDateTime, QVariant>;
QGenericReturnArgument GetReturnArgForQMetaType(int type, QMLReturnVariant& toInitialize);

QVariant VariantToQVariant(const Variant& var);
Variant QVariantToVariant(const QVariant& var);
Variant QMLReturnVariantToVariant(const QMLReturnVariant& var);

bool CanConvertArgTypes(const QMetaMethod& metaMethod, const std::vector<QVariant>& varargs);
bool GetMethodMetaForArgs(
    const QObject& obj, const std::string& method, const std::vector<QVariant>& varargs, QMetaMethod& ret);
std::vector<QGenericArgument> ConvertAndCreateQArgumentsForMethod(
    const QMetaMethod& metaMethod, std::vector<QVariant>& varargs);

} // namespace qt
} // namespace spix
