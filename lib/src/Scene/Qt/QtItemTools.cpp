/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtItemTools.h"

#include <QQmlContext>
#include <QQuickItem>
#include <iostream>

namespace spix {
namespace qt {

const QString repeater_class_name = QString("QQuickRepeater");
const char* item_at_method_name = "itemAt";

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index)
{
    QQuickItem* retVal = nullptr;
    bool success = QMetaObject::invokeMethod(
        repeater, item_at_method_name, Qt::DirectConnection, Q_RETURN_ARG(QQuickItem*, retVal), Q_ARG(int, index));
    if (success)
        return retVal;

    return nullptr;
}

QQuickItem* RepeaterChildWithName(QQuickItem* repeater, const QString& name)
{
    int itemIndex = 0;
    QQuickItem* item = nullptr;
    do {
        item = RepeaterChildAtIndex(repeater, itemIndex);
        if (item && getObjectId(item) == name) {
            break;
        }

        ++itemIndex;
    } while (item);

    return item;
}

QString getObjectId(QObject* object)
{
    if (object != nullptr) {
        // Allow to override id with objectName
        if (!object->objectName().isEmpty()) {
            return object->objectName();
        }
        QQmlContext* const context = qmlContext(object);
        if (context) {
            return context->nameForObject(object);
        }
    }

    // Fallback if item id is not found
    return object->objectName();
}

QObject* FindChildItem(QObject* object, const QString& name)
{
    if (auto qquickitem = qobject_cast<const QQuickItem*>(object)) {
        for (auto child : qquickitem->childItems()) {
            if (getObjectId(child) == name) {
                return child;
            }
            if (auto item = FindChildItem(child, name)) {
                return item;
            }
        }
    } else {
        for (auto child : object->children()) {
            if (getObjectId(child) == name) {
                return child;
            }
            if (auto item = FindChildItem(child, name)) {
                return item;
            }
        }
    }

    return nullptr;
}
} // namespace qt
} // namespace spix
