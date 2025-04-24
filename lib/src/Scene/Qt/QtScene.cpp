/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtScene.h"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

namespace {

QQuickWindow* getQQuickWindowWithName(const std::string& name)
{
    QString qtName = QString::fromStdString(name);
    QQuickWindow* foundWindow = nullptr;

    auto windows = QGuiApplication::topLevelWindows();
    for (const auto& window : windows) {
        QQuickWindow* qquickWindow = qobject_cast<QQuickWindow*>(window);
        if (qquickWindow && (spix::qt::GetObjectName(qquickWindow) == qtName)) {
            foundWindow = qquickWindow;
            break;
        }
    }

    return foundWindow;
}

QQuickItem* getQQuickItemWithSelector(const spix::ItemPath& path, QObject* root, const spix::path::PropertySelector& selector)
{
    const auto& propertyName = selector.name();
    QVariant propertyValue = root->property(propertyName.c_str());
    if (propertyValue.isValid()) {
        return propertyValue.value<QQuickItem*>();
    }
    return nullptr;
}

QQuickItem* getQQuickItemWithSelector(const spix::ItemPath& path, QObject* root, const spix::path::NameSelector& selector)
{
    const auto& itemName = selector.name();
    auto rootClassName = root->metaObject()->className();
    
    if (rootClassName == spix::qt::repeater_class_name) {
        QQuickItem* repeater = static_cast<QQuickItem*>(root);
        return spix::qt::RepeaterChildWithName(repeater, QString::fromStdString(itemName));
    } else {
        return spix::qt::FindChildItem<QQuickItem*>(root, itemName.c_str());
    }
}

template<typename T>
QQuickItem* getQQuickItemWithSelector(const spix::ItemPath&, QObject*, const T&)
{
    // Fallback for any unknown selector types
    return nullptr;
}

QQuickItem* getQQuickItemWithRoot(const spix::ItemPath& path, QObject* root)
{
    if (path.length() == 0) {
        return nullptr;
    }
    if (!root) {
        return nullptr;
    }

    const auto& itemComponent = path.rootComponent();
    const auto& itemSelector = itemComponent.selector();
    QQuickItem* subItem = nullptr;

    auto visitor = [&](const auto& selector) {
        return getQQuickItemWithSelector(path, root, selector);
    };
    
    subItem = std::visit(visitor, itemSelector);

    if (!subItem) {
        return nullptr;
    }

    if (path.length() == 1) {
        return subItem;
    }

    return getQQuickItemWithRoot(path.subPath(1), subItem);
}

QQuickWindow* getQQuickWindowAtPathRoot(const spix::ItemPath& path)
{
    const auto& windowPathComponent = path.rootComponent();
    const auto& windowSelector = windowPathComponent.selector();
    if (!std::holds_alternative<spix::path::NameSelector>(windowSelector)) {
        return nullptr;
    }

    const auto& windowName = std::get<spix::path::NameSelector>(windowSelector).name();
    QQuickWindow* itemWindow = getQQuickWindowWithName(windowName);
    
    return itemWindow;
}

QQuickItem* getQQuickItemAtPath(const spix::ItemPath& path)
{
    QQuickWindow* itemWindow = getQQuickWindowAtPathRoot(path);
    QQuickItem* item = nullptr;

    if (!itemWindow) {
        return nullptr;
    }

    if (path.length() > 1) {
        item = getQQuickItemWithRoot(path.subPath(1), itemWindow);
    } else {
        item = itemWindow->contentItem();
    }

    return item;
}

} // namespace

namespace spix {

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    QQuickWindow* itemWindow = getQQuickWindowAtPathRoot(path);

    if (!itemWindow || !itemWindow->contentItem()) {
        return {};
    }
    if (path.length() <= 1) {
        return std::make_unique<QtItem>(itemWindow);
    }

    auto item = getQQuickItemWithRoot(path.subPath(1), itemWindow);

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
    auto item = getQQuickItemAtPath(targetItem);
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
    auto item = getQQuickItemAtPath(targetItem);
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

} // namespace spix
