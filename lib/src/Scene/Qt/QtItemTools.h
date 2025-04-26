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

#include <functional>

class QString;

namespace spix {
namespace qt {

extern const QString repeater_class_name;
extern const char* item_at_method_name;

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index);

QString GetObjectName(QObject* object);

/**
 * @brief Returns the simplified type name of a QObject
 * 
 * Removes QQuick prefix and _QML suffix from the class name
 * 
 * @param object The QObject to get the type from
 * @return The simplified type name
 */
QString TypeStringForObject(QObject* object);

/**
 * @brief Iterates over all children of a QObject, with special handling for QQuickItems and Repeaters
 *
 * This function handles the different ways children are accessed in Qt:
 * - For QQuickItems, it uses childItems() instead of children()
 * - For QQuickRepeaters, it iterates through its generated items
 * - For regular QObjects, it uses the children() list
 *
 * @param object The parent object whose children to iterate over
 * @param callback A function to call for each child, should return true to continue iteration or false to stop
 */
void ForEachChild(QObject* object, const std::function<bool(QObject*)>& callback);

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
