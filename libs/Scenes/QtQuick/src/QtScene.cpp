/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtScene.h"
#include "FindQtItem.h"

#include <QtItem.h>
#include <QtItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

namespace {

QJsonObject serializeItem(QObject* object)
{
    QJsonObject node;

    node["type"] = spix::qt::TypeStringForObject(object);
    node["objectName"] = spix::qt::GetObjectName(object);
    node["className"] = QString(object->metaObject()->className());

    auto quickItem = qobject_cast<QQuickItem*>(object);
    if (quickItem) {
        auto globalPos = quickItem->mapToGlobal(QPointF(0, 0));
        QJsonObject bounds;
        bounds["x"] = globalPos.x();
        bounds["y"] = globalPos.y();
        bounds["width"] = quickItem->width();
        bounds["height"] = quickItem->height();
        node["bounds"] = bounds;

        node["visible"] = quickItem->isVisible();
        node["opacity"] = quickItem->opacity();
        node["clip"] = quickItem->clip();
        node["enabled"] = quickItem->isEnabled();
    }

    // Serialize QMetaObject properties
    QJsonObject properties;
    const QMetaObject* meta = object->metaObject();
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);

        // Skip non-readable or object-pointer properties
        if (!prop.isReadable()) {
            continue;
        }

        int propType = prop.userType();
        // Skip QObject* and derived pointer types
        if (QMetaType(propType).flags() & QMetaType::PointerToQObject) {
            continue;
        }

        QVariant value = prop.read(object);
        if (value.canConvert<QString>()) {
            properties[prop.name()] = value.toString();
        }
    }
    node["properties"] = properties;

    // Recurse into children
    QJsonArray children;
    spix::qt::ForEachChild(object, [&](QObject* child) -> bool {
        children.append(serializeItem(child));
        return true;
    });
    node["children"] = children;

    return node;
}

} // anonymous namespace

namespace spix {

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    auto window = qt::GetQQuickWindowAtPath(path);

    if (!window || !window->contentItem()) {
        return {};
    }
    if (path.length() == 1) {
        return std::make_unique<QtItem>(window);
    }

    auto item = qt::GetQQuickItemAtPath(path);

    if (!item) {
        return {};
    }

    return std::make_unique<QtItem>(item);
}

Events& QtScene::events()
{
    return m_events;
}

void QtScene::takeScreenshot(const ItemPath& targetItem, const std::string& filePath)
{
    auto item = qt::GetQQuickItemAtPath(targetItem);
    if (!item) {
        return;
    }

    // take screenshot of the full window
    auto windowImage = item->window()->grabWindow();

    // get the rect of the item in window space in pixels, account for the device pixel ratio
    QRectF imageCropRectItemSpace {0, 0, item->width(), item->height()};
    auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
    QRect imageCropRect(imageCropRectF.x() * windowImage.devicePixelRatio(),
        imageCropRectF.y() * windowImage.devicePixelRatio(), imageCropRectF.width() * windowImage.devicePixelRatio(),
        imageCropRectF.height() * windowImage.devicePixelRatio());

    // crop the window image to the item rect
    auto image = windowImage.copy(imageCropRect);
    image.save(QString::fromStdString(filePath));
}

std::string QtScene::takeScreenshotAsBase64(const ItemPath& targetItem)
{
    auto item = qt::GetQQuickItemAtPath(targetItem);
    if (!item) {
        return "";
    }

    // take screenshot of the full window
    auto windowImage = item->window()->grabWindow();

    // get the rect of the item in window space in pixels, account for the device pixel ratio
    QRectF imageCropRectItemSpace {0, 0, item->width(), item->height()};
    auto imageCropRectF = item->mapRectToScene(imageCropRectItemSpace);
    QRect imageCropRect(imageCropRectF.x() * windowImage.devicePixelRatio(),
        imageCropRectF.y() * windowImage.devicePixelRatio(), imageCropRectF.width() * windowImage.devicePixelRatio(),
        imageCropRectF.height() * windowImage.devicePixelRatio());

    // crop the window image to the item rect
    auto image = windowImage.copy(imageCropRect);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    buffer.close();

    return byteArray.toBase64().toStdString();
}

std::string QtScene::dumpTree(const ItemPath& rootPath)
{
    QQuickItem* rootItem = nullptr;

    if (rootPath.length() == 0) {
        return "{}";
    }

    if (rootPath.length() == 1) {
        auto window = qt::GetQQuickWindowAtPath(rootPath);
        if (window) {
            rootItem = window->contentItem();
        }
    } else {
        rootItem = qt::GetQQuickItemAtPath(rootPath);
    }

    if (!rootItem) {
        return "{}";
    }

    QJsonObject tree = serializeItem(rootItem);
    QJsonDocument doc(tree);
    return doc.toJson(QJsonDocument::Compact).toStdString();
}

} // namespace spix
