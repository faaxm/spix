/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/Data/Geometry.h>

namespace spix {

Size::Size()
: width(0.0)
, height(0.0)
{
}

Size::Size(Real width, Real height)
: width(width)
, height(height)
{
}

Point::Point()
: x(0.0)
, y(0.0)
{
}

Point::Point(Real x, Real y)
: x(x)
, y(y)
{
}

Rect::Rect()
{
}

Rect::Rect(Real x, Real y, Real width, Real height)
: topLeft(x, y)
, size(width, height)
{
}

} // namespace spix
