/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Geometry.h>

#include <string>

namespace spix {

/**
 * @brief Represents an item in a scene
 *
 * This object can be queried for basic properties of an item in the scene.
 * It will be implemented by the different backends, depending on whether this
 * is a Qml/Qt/Mock or other scene.
 */
class Item {
public:
    virtual ~Item() = default;

    // Item properties
    virtual Size size() const = 0;
    virtual std::string stringProperty(const std::string& name) const = 0;
    virtual void setStringProperty(const std::string& name, const std::string& value) = 0;
    virtual bool visible() const = 0;
};

} // namespace spix
