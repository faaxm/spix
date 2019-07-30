/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

class QObject;
struct QMetaObject;
class QQuickItem;

namespace spix {
namespace utils {

void DumpObject(const QObject* obj);
void DumpObjectTree(const QObject* obj, int level = 0);
void DumpMetaObject(const QMetaObject* meta);
void DumpQQuickItemTree(const QQuickItem* item, int level = 0);
void DumpQQuickItemOrQObjectTree(const QObject* object, int level = 0);

} // namespace utils
} // namespace spix
