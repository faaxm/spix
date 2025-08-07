/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

namespace spix {

using Real = double;

struct SPIXCORE_EXPORT Size {
    Size();
    Size(Real width, Real height);

    Real width;
    Real height;
};

struct SPIXCORE_EXPORT Point {
    Point();
    Point(Real x, Real y);

    Real x;
    Real y;
};

struct SPIXCORE_EXPORT Rect {
    Rect();
    Rect(Real x, Real y, Real width, Real height);

    Point topLeft;
    Size size;
};

} // namespace spix
