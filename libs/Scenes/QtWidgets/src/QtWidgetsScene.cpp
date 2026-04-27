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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QObject>
#include <QPixmap>
#include <QWidget>

namespace {

QJsonObject serializeWidget(QObject* object)
{
    QJsonObject node;

    node["type"] = spix::qt::TypeStringForWidget(object);
    node["objectName"] = spix::qt::GetObjectName(object);
    node["className"] = QString(object->metaObject()->className());

    auto widget = qobject_cast<QWidget*>(object);
    if (widget) {
        auto globalPos = widget->mapToGlobal(QPoint(0, 0));
        QJsonObject bounds;
        bounds["x"] = globalPos.x();
        bounds["y"] = globalPos.y();
        bounds["width"] = widget->width();
        bounds["height"] = widget->height();
        node["bounds"] = bounds;

        node["visible"] = widget->isVisible();
        node["enabled"] = widget->isEnabled();
    }

    // Serialize QMetaObject properties
    QJsonObject properties;
    const QMetaObject* meta = object->metaObject();
    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);

        if (!prop.isReadable()) {
            continue;
        }

        int propType = prop.userType();
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
        children.append(serializeWidget(child));
        return true;
    });
    node["children"] = children;

    return node;
}

} // anonymous namespace

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

std::string QtWidgetsScene::dumpTree(const ItemPath& rootPath)
{
    QWidget* rootWidget = nullptr;

    if (rootPath.length() == 0) {
        return "{}";
    }

    if (rootPath.length() == 1) {
        rootWidget = qt::GetTopLevelWidgetAtPath(rootPath);
    } else {
        rootWidget = qt::GetQWidgetAtPath(rootPath);
    }

    if (!rootWidget) {
        return "{}";
    }

    QJsonObject tree = serializeWidget(rootWidget);
    QJsonDocument doc(tree);
    return doc.toJson(QJsonDocument::Compact).toStdString();
}

} // namespace spix
