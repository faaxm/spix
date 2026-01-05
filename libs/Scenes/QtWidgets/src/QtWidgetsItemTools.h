/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Variant.h>

#include <QDateTime>
#include <QObject>
#include <QVariant>

#include <functional>

class QString;
class QWidget;

namespace spix {
namespace qt {

/**
 * @brief Returns the object name of a QObject
 *
 * For widgets, this simply returns the objectName property.
 *
 * @param object The QObject to get the name from
 * @return The object name
 */
QString GetObjectName(QObject* object);

/**
 * @brief Returns the simplified type name of a QWidget
 *
 * Removes Q prefix from the class name (e.g., QPushButton -> PushButton)
 *
 * @param object The QObject to get the type from
 * @return The simplified type name
 */
QString TypeStringForWidget(QObject* object);

/**
 * @brief Iterates over all child widgets of a QWidget
 *
 * This function iterates through the direct child widgets using
 * findChildren with Qt::FindDirectChildrenOnly flag.
 *
 * @param object The parent object whose children to iterate over
 * @param callback A function to call for each child, should return true to continue iteration or false to stop
 */
void ForEachChild(QObject* object, const std::function<bool(QObject*)>& callback);

// Qt-generic utilities (same as QtQuick)
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
