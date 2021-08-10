/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "spix_export.h"

namespace spix {

using Real = double;

struct SPIX_EXPORT Size {
    Size();
    Size(Real width, Real height);

    Real width;
    Real height;
};

struct SPIX_EXPORT Point {
    Point();
    Point(Real x, Real y);

    Real x;
    Real y;
};

struct SPIX_EXPORT Rect {
    Rect();
    Rect(Real x, Real y, Real width, Real height);

    Point topLeft;
    Size size;
};

} // namespace spix
