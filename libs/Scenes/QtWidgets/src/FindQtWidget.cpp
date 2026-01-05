/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "FindQtWidget.h"
#include <QtWidgetsItemTools.h>
#include <Spix/Data/ItemPathComponent.h>

#include <QApplication>
#include <QWidget>

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
    if (spix::qt::TypeStringForWidget(item) == QString::fromStdString(typeName)) {
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
 * Performs a DFS to find the first matching widget in the UI tree.
 *
 * @param pathComponents Vector of path components to match
 * @param currentNode Starting node for the search
 * @param matchedCount Number of path components already matched in the ancestor chain
 * @return The first matching QWidget or nullptr if none found
 */
QWidget* FindMatchingWidget(
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

        // If we've matched all components, return this item if it's a QWidget
        if (matchedCount == pathComponents.size()) {
            return qobject_cast<QWidget*>(matchedObject);
        }

        // Continue searching in the matched object's hierarchy
        // - if it is different from the current node or
        // - if the next component is a property selector, as a property selector might reference a property of the
        //   current node
        const auto& nextComponent = pathComponents[matchedCount];
        if (matchedObject != currentNode
            || std::holds_alternative<spix::path::PropertySelector>(nextComponent.selector())) {
            return FindMatchingWidget(pathComponents, matchedObject, matchedCount);
        }
    }

    // Continue DFS through children
    QWidget* result = nullptr;

    // Use ForEachChild to iterate through all children
    spix::qt::ForEachChild(currentNode, [&](QObject* child) -> bool {
        if ((result = FindMatchingWidget(pathComponents, child, matchedCount))) {
            return false; // Stop iteration if we found a match
        }
        return true; // Continue iteration
    });

    return result; // Will return nullptr if no match was found
}

/**
 * Find a top-level QWidget by its name
 * @param name The name of the widget to find
 * @return The widget if found, nullptr otherwise
 */
QWidget* GetTopLevelWidgetWithName(const std::string& name)
{
    QString qtName = QString::fromStdString(name);
    QWidget* foundWidget = nullptr;

    auto widgets = QApplication::topLevelWidgets();
    for (const auto& widget : widgets) {
        if (spix::qt::GetObjectName(widget) == qtName) {
            foundWidget = widget;
            break;
        }
    }

    return foundWidget;
}

} // namespace

namespace spix {
namespace qt {

QWidget* GetTopLevelWidgetAtPath(const spix::ItemPath& path)
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
    QWidget* widget = GetTopLevelWidgetWithName(windowName);

    if (!widget) {
        return nullptr;
    }

    return widget;
}

QWidget* GetQWidgetAtPath(const spix::ItemPath& path)
{
    if (path.length() == 0) {
        return nullptr;
    }

    QWidget* topLevelWidget = GetTopLevelWidgetAtPath(path);

    if (!topLevelWidget) {
        return nullptr;
    }

    // If path only has window component, return the top-level widget
    if (path.length() == 1) {
        return topLevelWidget;
    }

    // Start DFS from the top-level widget to find the target widget
    // Skip the window component (index 0) and start matching from the first child component
    auto components = path.components();
    auto result = FindMatchingWidget(components, topLevelWidget, 1);

    return result;
}

} // namespace qt
} // namespace spix
