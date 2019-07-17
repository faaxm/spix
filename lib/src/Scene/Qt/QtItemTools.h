/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

class QQuickItem;
class QString;
class QLatin1String;

namespace spix {
namespace qt {

extern const QLatin1String repeater_class_name, listview_class_name, gridview_class_name;
extern const char* item_at_method_name;

QQuickItem* RepeaterChildAtIndex(QQuickItem* repeater, int index);
QQuickItem* RepeaterChildWithName(QQuickItem* repeater, const QString& name);
QQuickItem* ListViewChildAtIndex(QQuickItem* repeater, int index);
QQuickItem* ListViewChildWithName(QQuickItem* repeater, const QString& name);

} // namespace qt
} // namespace spix
