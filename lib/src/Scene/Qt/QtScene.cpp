/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtScene.h"
#include "FindQtItem.h"

#include <Scene/Qt/QtItem.h>
#include <Scene/Qt/QtItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QBuffer>
#include <QByteArray>
#include <QGuiApplication>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

namespace spix {

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    // Find item within window
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

} // namespace spix
