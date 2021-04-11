/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/Geometry.h>

namespace spix {

Size::Size(Real width, Real height)
: width(width)
, height(height)
{
}

Point::Point(Real x, Real y)
: x(x)
, y(y)
{
}

Rect::Rect(Real x, Real y, Real width, Real height)
: topLeft(x, y)
, size(width, height)
{
}

} // namespace spix
