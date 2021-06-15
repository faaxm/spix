/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <QObject>
#include <QQuickItem>

class QString;

namespace spix {
namespace qt {

extern const QString repeater_class_name;
extern const char* item_at_method_name;

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index);
QQuickItem* RepeaterChildWithName(QQuickItem* repeater, const QString& name);

QString getObjectId(QObject* object);

/**
 * @brief Find a child object with the given name.
 *
 * This works similar to `QObject::findChild`. However, once it
 * encounters a `QQuickItem`, it no longer iterates over the object's
 * `children()`, but rather its `childItems()`.
 */
QObject* FindChildItem(QObject* object, const QString& name);

template <typename T>
T FindChildItem(QObject* object, const QString& name)
{
    return qobject_cast<T>(FindChildItem(object, name));
}
} // namespace qt
} // namespace spix
