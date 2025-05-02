# Item Path System in Spix

## Abstract

This document provides an overview of the Item Path system used within the Spix framework. It explains how paths are structured and used to identify UI elements from a user's perspective. Additionally, it offers a high-level glimpse into the underlying implementation concepts to guide potential extensions or modifications.

## User Perspective: Addressing UI Elements

In Spix, UI elements within a scene (like a QML application) are identified using an `ItemPath`. An `ItemPath` is conceptually similar to a file system path, using forward slashes (`/`) to separate components that describe a hierarchical structure.

The path starts with the name of the root element, typically the main window. Subsequent components specify how to traverse the UI tree to find the desired element. Each component uses a specific selector type to identify the next element in the sequence.

Importantly, the path does not need to specify every single element in the hierarchy. You can skip intermediate elements. Spix will perform a search (typically depth-first) starting from the element matched by the previous component to find the first descendant that matches the next component in the path. For example, a path like `mainWindow/myButton` can successfully find the button even if its actual full path is `mainWindow/someContainer/anotherLayer/myButton`.

Available selector types are:

*   **Name Selector**: Matches an element by its `objectName` (or equivalent identifier). This is the default selector type if no prefix is used.
    *   Example: `mainWindow/myButton`
*   **Property Selector**: Matches an element accessible via a property of the current element. The selector starts with a dot (`.`).
    *   Example: `mainWindow/parentContainer/.contentItem`
*   **Type Selector**: Matches an element based on its class or type name. The selector starts with a hash (`#`).
    *   Example: `mainWindow/#Button`
*   **Value Selector**: Matches an element based on its primary textual value (e.g., the `text` property). The selector is enclosed in double quotes (`"`).
    *   Example: `mainWindow/userList/"Alice"`
*   **Property Value Selector**: Matches an element based on a specific property having a specific string value. The selector is enclosed in parentheses `()` with an equals sign (`=`) separating the property name and value.
    *   Example: `mainWindow/buttons/(enabled=true)`

These selectors can be combined to create complex paths that navigate the UI tree effectively. For example, `mainWindow/userList/#CustomDelegate/(name=Bob)/.detailsButton` finds a button that is assigned to the `detailsButton` property of an element with property "name" set to "Bob" within an element of type `CustomDelegate`, located within an element named `userList` inside `mainWindow`.

## Implementation Overview

Internally, an `ItemPath` is represented as a `std::vector` of `spix::path::Component` objects. Each `Component` encapsulates a specific selector type.

The different selector types (`NameSelector`, `PropertySelector`, `TypeSelector`, `ValueSelector`, `PropertyValueSelector`) are managed using a `std::variant<...>`. This allows a `Component` object to hold any one of the possible selector types in a type-safe manner.

When Spix needs to locate an item based on an `ItemPath` (e.g., in `spix::qt::GetQQuickItemAtPath`), it typically processes the path components sequentially. The process often involves a search algorithm, like Depth-First Search (DFS), starting from a root element (like a window's content item).

For each component in the path, the search algorithm attempts to find a matching child (or related item, in the case of property selectors) based on the component's selector type. The use of `std::variant` for selectors facilitates this matching process. A common pattern, as seen in `FindQtItem.cpp`, involves using `std::visit` on the `Component::selector()` variant. `std::visit` allows dispatching to the correct matching logic based on the actual selector type held by the variant at runtime, without requiring complex conditional chains.

This design allows for easy extension: adding a new selector type involves defining a new selector class, adding it to the `Selector` variant, and implementing the corresponding matching logic within the visitor function used with `std::visit`.
