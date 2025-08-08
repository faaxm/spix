/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "FindQtItem.h"
#include <QtItemTools.h>
#include <Spix/Data/ItemPathComponent.h>

#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>

namespace {

template <typename SelectorType>
QObject* MatchesSpecificSelector(QObject* item, const SelectorType& specific_selector)
{
    // Default implementation for unknown selector types
    return nullptr;
}

template <>
QObject* MatchesSpecificSelector(QObject* item, const spix::path::NameSelector& specific_selector)
{
    const auto& itemName = specific_selector.name();
    if (spix::qt::GetObjectName(item) == QString::fromStdString(itemName)) {
        return item;
    }
    return nullptr;
}

template <>
QObject* MatchesSpecificSelector(QObject* item, const spix::path::PropertySelector& specific_selector)
{
    if (!item) {
        return nullptr;
    }
    const auto& propertyName = specific_selector.name();
    QVariant propertyValue = item->property(propertyName.c_str());

    if (!propertyValue.isValid()) {
        return nullptr;
    }

    return propertyValue.value<QObject*>();
}

template <>
QObject* MatchesSpecificSelector(QObject* item, const spix::path::TypeSelector& specific_selector)
{
    if (!item) {
        return nullptr;
    }

    const auto& typeName = specific_selector.type();
    if (spix::qt::TypeStringForObject(item) == QString::fromStdString(typeName)) {
        return item;
    }
    return nullptr;
}

template <>
QObject* MatchesSpecificSelector(QObject* item, const spix::path::ValueSelector& specific_selector)
{
    if (!item) {
        return nullptr;
    }

    // Check if the "text" property exists and matches the specified value
    QVariant textProperty = item->property("text");
    if (textProperty.isValid() && textProperty.canConvert<QString>()) {
        QString textValue = textProperty.toString();
        if (textValue == QString::fromStdString(specific_selector.value())) {
            return item;
        }
    }
    return nullptr;
}

template <>
QObject* MatchesSpecificSelector(QObject* item, const spix::path::PropertyValueSelector& specific_selector)
{
    if (!item) {
        return nullptr;
    }

    // Get the property name and expected value
    const auto& propertyName = specific_selector.propertyName();
    const auto& expectedValue = specific_selector.propertyValue();

    // Check if the property exists
    QVariant propertyValue = item->property(propertyName.c_str());
    if (propertyValue.isValid() && propertyValue.canConvert<QString>()) {
        QString actualValue = propertyValue.toString();
        if (actualValue == QString::fromStdString(expectedValue)) {
            return item;
        }
    }
    return nullptr;
}

QObject* MatchesSelector(QObject* item, const spix::path::Selector& selector)
{
    return std::visit(
        [item](const auto& specific_selector) -> QObject* { return MatchesSpecificSelector(item, specific_selector); },
        selector);
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
    const std::vector<spix::path::Component>& pathComponents, QObject* currentNode, size_t matchedCount)
{
    if (!currentNode) {
        return nullptr;
    }
    if (matchedCount >= pathComponents.size()) {
        return nullptr;
    }

    // If we have a potential match, check if this node matches the next component
    const auto& component = pathComponents[matchedCount];
    const auto& selector = component.selector();

    // Check if this node matches the current selector
    QObject* matchedObject = MatchesSelector(currentNode, selector);
    if (matchedObject) {
        // Increment matched count as we found a match
        matchedCount++;

        // If we've matched all components, return this item if it's a QQuickItem
        if (matchedCount == pathComponents.size()) {
            return qobject_cast<QQuickItem*>(matchedObject);
        }

        // Continue searching in the matched object's hierarchy
        // - if it is different from the current node or
        // - if the next component is a property selector, as a property selector might reference a property of the
        //   current node
        const auto& nextComponent = pathComponents[matchedCount];
        if (matchedObject != currentNode
            || std::holds_alternative<spix::path::PropertySelector>(nextComponent.selector())) {
            return FindMatchingItem(pathComponents, matchedObject, matchedCount);
        }
    }

    // Continue DFS through children
    QQuickItem* result = nullptr;

    // Use ForEachChild to iterate through all children
    spix::qt::ForEachChild(currentNode, [&](QObject* child) -> bool {
        if ((result = FindMatchingItem(pathComponents, child, matchedCount))) {
            return false; // Stop iteration if we found a match
        }
        return true; // Continue iteration
    });

    return result; // Will return nullptr if no match was found
}

/**
 * Find a QQuickWindow by its name
 * @param name The name of the window to find
 * @return The window if found, nullptr otherwise
 */
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

} // namespace

namespace spix {
namespace qt {

QQuickWindow* GetQQuickWindowAtPath(const spix::ItemPath& path)
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

    return window;
}

QQuickItem* GetQQuickItemAtPath(const spix::ItemPath& path)
{
    if (path.length() == 0) {
        return nullptr;
    }

    QQuickWindow* window = GetQQuickWindowAtPath(path);

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
    auto result = FindMatchingItem(components, window->contentItem(), 1);
    if (!result) {
        // go through window's children() rather than its contentItem(). This includes Dialogs.
        result = FindMatchingItem(components, window, 1);
    }

    return result;
}

} // namespace qt
} // namespace spix
