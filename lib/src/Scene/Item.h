/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Geometry.h>

#include <string>

namespace spix {

class Item {
public:
    virtual ~Item() = default;

    // Item properties
    virtual Size size() const = 0;
    virtual std::string stringProperty(const std::string& name) const = 0;
    virtual bool visible() const = 0;
};

} // namespace spix
