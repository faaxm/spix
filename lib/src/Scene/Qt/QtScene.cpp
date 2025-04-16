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

QString getNameForObject(QObject* object)
{
    QString name;
    if (spix::qt::PropertValueByObject(object, QString::fromStdString("text")) != "") {
        name = "\"" + spix::qt::PropertValueByObject(object, QString::fromStdString("text")) + "\"";
    } else if (spix::qt::PropertValueByObject(object, QString::fromStdString("source")) != "") {
        name = "(source=" + spix::qt::PropertValueByObject(object, QString::fromStdString("source")) + ")";
    } else if (spix::qt::GetObjectName(object) != "") {
        name = spix::qt::GetObjectName(object);
    } else {
        name = "#" + spix::qt::TypeByObject(object);
    }

    return name;
}

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

QQuickItem* getQQuickItemWithRoot(const spix::ItemPath& path, QObject* root)
{
    if (path.length() == 0) {
        return nullptr;
    }
    if (!root) {
        return nullptr;
    }

    auto rootClassName = root->metaObject()->className();
    auto itemName = path.rootComponent();
    QQuickItem* subItem = nullptr;
    QVector<QObject*> subItems = {};

    if (itemName.compare(0, 1, ".") == 0) {
        auto propertyName = itemName.substr(1);
        QVariant propertyValue = root->property(propertyName.c_str());
        if (propertyValue.isValid()) {
            subItem = propertyValue.value<QQuickItem*>();
        }

    } else if (itemName.compare(0, 1, "\"") == 0) {
        // remove ""
        size_t found = itemName.find("\"");
        auto searchText = itemName.substr(found + 1, itemName.length() - 2);

        subItem = spix::qt::FindChildItem<QQuickItem*>(
            root, itemName.c_str(), QString::fromStdString("text"), QString::fromStdString(searchText), {});

    } else if (itemName.compare(0, 1, "#") == 0) {
        // remove #
        size_t found = itemName.find("#");
        auto type = QString::fromStdString(itemName.substr(found + 1));

        if (path.length() > 1) {
            subItems = spix::qt::FindChildItems(root, type);

            for (const auto item : subItems) {
                auto foundItem = getQQuickItemWithRoot(path.subPath(1), item);
                if (foundItem != nullptr) {
                    return foundItem;
                }
            }
        } else {
            subItem = spix::qt::FindChildItem<QQuickItem*>(root, {}, {}, {}, type);
            return subItem;
        }
    } else if (itemName.compare(0, 1, "(") == 0) {
        // remove ()
        size_t foundBracketSign = itemName.find('(');
        auto searchText = itemName.substr(foundBracketSign + 1, itemName.length() - 2);

        // Split in to property and value
        size_t foundEqualSign = searchText.find('=');
        auto property = QString::fromStdString(searchText.substr(0, foundEqualSign));
        auto value = QString::fromStdString(searchText.substr(foundEqualSign + 1));

        subItem = spix::qt::FindChildItem<QQuickItem*>(root, itemName.c_str(), property, value, {});
    } else if (rootClassName == spix::qt::repeater_class_name) {
        QQuickItem* repeater = static_cast<QQuickItem*>(root);
        subItem = spix::qt::RepeaterChildWithName(repeater, QString::fromStdString(itemName));
    } else {
        subItem = spix::qt::FindChildItem<QQuickItem*>(root, itemName.c_str());
    }

    if (path.length() == 1) {
        return subItem;
    }

    return getQQuickItemWithRoot(path.subPath(1), subItem);
}

QQuickItem* getQQuickItemAtPath(const spix::ItemPath& path)
{
    auto windowName = path.rootComponent();
    QQuickWindow* itemWindow = getQQuickWindowWithName(windowName);
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
    auto windowName = path.rootComponent();
    QQuickWindow* itemWindow = getQQuickWindowWithName(windowName);

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
