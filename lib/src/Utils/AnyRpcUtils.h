/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Variant.h>
#include <anyrpc/anyrpc.h>

namespace spix {
namespace utils {

Variant AnyRPCValueToVariant(const anyrpc::Value& valueConst);
anyrpc::Value VariantToAnyRPCValue(const Variant& value);

} // namespace utils
} // namespace spix
