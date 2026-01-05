/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "QtWidgetsScene.h"
#include "FindQtWidget.h"

#include <QtWidgetsItem.h>
#include <QtWidgetsItemTools.h>
#include <Spix/Data/ItemPath.h>

#include <QApplication>
#include <QBuffer>
#include <QByteArray>
#include <QObject>
#include <QPixmap>
#include <QWidget>

namespace spix {

std::unique_ptr<Item> QtWidgetsScene::itemAtPath(const ItemPath& path)
{
    auto widget = qt::GetQWidgetAtPath(path);

    if (!widget) {
        return {};
    }

    return std::make_unique<QtWidgetsItem>(widget);
}

Events& QtWidgetsScene::events()
{
    return m_events;
}

void QtWidgetsScene::takeScreenshot(const ItemPath& targetItem, const std::string& filePath)
{
    auto widget = qt::GetQWidgetAtPath(targetItem);
    if (!widget) {
        return;
    }

    // QWidget::grab() captures the widget directly - no need for window grabbing and cropping
    QPixmap pixmap = widget->grab();
    pixmap.save(QString::fromStdString(filePath));
}

std::string QtWidgetsScene::takeScreenshotAsBase64(const ItemPath& targetItem)
{
    auto widget = qt::GetQWidgetAtPath(targetItem);
    if (!widget) {
        return "";
    }

    // QWidget::grab() captures the widget directly
    QPixmap pixmap = widget->grab();

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    buffer.close();

    return byteArray.toBase64().toStdString();
}

} // namespace spix
