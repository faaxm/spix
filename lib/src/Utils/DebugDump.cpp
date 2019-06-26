/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DebugDump.h"

#include <QMetaObject>
#include <QObject>
#include <QQuickItem>

#include <iostream>

namespace {
void helper_indentbylevel(int level)
{
    for (int i = 0; i < level; ++i) {
        std::cout << ". ";
    }
}
} // namespace

namespace spix {
namespace utils {

void DumpObject(const QObject* obj)
{
    auto meta = obj->metaObject();

    std::cout << "\"" << obj->objectName().toStdString() << "\" [" << meta->className();
    /*for(int i = 0; i < meta->propertyCount(); ++i){
        auto property = meta->property(i);
        std::cout << ", prop: " << property.name() << "=" << obj->property(property.name()).toString().toStdString();
    }*/
    std::cout << "]";
}

void DumpObjectTree(const QObject* obj, int level)
{
    helper_indentbylevel(level);
    DumpObject(obj);
    std::cout << std::endl;

    for (auto item : obj->children()) {
        DumpObjectTree(item, level + 1);
    }
}

void DumpMetaObject(const QMetaObject* meta)
{
    // Dump methods
    std::cout << "Methods:" << std::endl;
    for (int i = 0; i < meta->methodCount(); ++i) {
        auto method = meta->method(i);
        auto signatureBytes = method.methodSignature();
        std::cout << "-- ( " << method.typeName() << " )  " << QString::fromLatin1(signatureBytes).toStdString()
                  << std::endl;
    }
}

void DumpQQuickItemTree(const QQuickItem* item, int level)
{
    helper_indentbylevel(level);

    DumpObject(item);
    std::cout << std::endl;

    for (auto child : item->childItems()) {
        DumpQQuickItemTree(child, level + 1);
    }
}

} // namespace utils
} // namespace uibot
