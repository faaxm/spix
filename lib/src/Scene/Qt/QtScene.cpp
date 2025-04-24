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

QQuickWindow* GetQQuickWindowWithName(const std::string& name)
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

bool MatchesSelector(QObject* item, const spix::path::Selector& selector) 
{
    return std::visit([item](const auto& specific_selector) -> bool {
        using SelectorType = std::decay_t<decltype(specific_selector)>;
        
        if constexpr (std::is_same_v<SelectorType, spix::path::NameSelector>) {
            const auto& itemName = specific_selector.name();
            return spix::qt::GetObjectName(item) == QString::fromStdString(itemName);
        }
        else if constexpr (std::is_same_v<SelectorType, spix::path::PropertySelector>) {
            const auto& propertyName = specific_selector.name();
            QVariant propertyValue = item->property(propertyName.c_str());
            return propertyValue.isValid();
        }
        else {
            return false; // Unknown selector type
        }
    }, selector);
}

/**
 * Performs a DFS to find the first matching item in the UI tree.
 * 
 * @param pathComponents Vector of path components to match
 * @param currentNode Starting node for the search
 * @param matchedCount Number of path components already matched in the ancestor chain
 * @return The first matching QQuickItem or nullptr if none found
 */
QQuickItem* FindMatchingItem(
    const std::vector<spix::path::Component>& pathComponents,
    QObject* currentNode,
    size_t matchedCount
) {
    if (!currentNode) {
        return nullptr;
    }

    // If we have a potential match, check if this node matches the next component
    if (matchedCount < pathComponents.size()) {
        const auto& nextComponent = pathComponents[matchedCount];
        if (MatchesSelector(currentNode, nextComponent.selector())) {
            matchedCount++;
            
            // If we've matched all components, return this item if it's a QQuickItem
            if (matchedCount == pathComponents.size()) {
                return qobject_cast<QQuickItem*>(currentNode);
            }
        }
    }
    
    // Continue DFS through children
    QQuickItem* result = nullptr;
    
    // Use ForEachChild to iterate through all children, with special handling for repeaters
    spix::qt::ForEachChild(currentNode, [&](QObject* child) -> bool {
        if ((result = FindMatchingItem(pathComponents, child, matchedCount))) {
            return false; // Stop iteration if we found a match
        }
        return true; // Continue iteration
    });
    
    return result; // Will return nullptr if no match was found
}

QQuickItem* GetQQuickItemAtPath(const spix::ItemPath& path)
{
    if (path.length() == 0) {
        return nullptr;
    }
    
    // First find the window
    const auto& windowComponent = path.rootComponent();
    const auto& windowSelector = windowComponent.selector();
    
    if (!std::holds_alternative<spix::path::NameSelector>(windowSelector)) {
        return nullptr;
    }
    
    const auto& windowName = std::get<spix::path::NameSelector>(windowSelector).name();
    QQuickWindow* window = GetQQuickWindowWithName(windowName);
    
    if (!window) {
        return nullptr;
    }
    
    // If path only has window component, return window's contentItem
    if (path.length() == 1) {
        return window->contentItem();
    }
    
    // Start DFS from window's contentItem to find the item
    // Skip the window component (index 0) and start matching from the first child component
    auto components = path.components();
    return FindMatchingItem(components, window->contentItem(), 1);
}

} // namespace

namespace spix {

std::unique_ptr<Item> QtScene::itemAtPath(const ItemPath& path)
{
    if (path.length() == 0) {
        return {};
    }
    
    // Find window first
    QQuickWindow* window = GetQQuickWindowWithName(
        std::get<path::NameSelector>(path.rootComponent().selector()).name());
    
    if (!window) {
        return {};
    }
    
    // If only window in path, return window
    if (path.length() == 1) {
        return std::make_unique<QtItem>(window);
    }
    
    // Find item within window via DFS
    auto components = path.components();
    auto item = FindMatchingItem(components, window->contentItem(), 1);
    
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
    auto item = GetQQuickItemAtPath(targetItem);
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
    auto item = GetQQuickItemAtPath(targetItem);
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
