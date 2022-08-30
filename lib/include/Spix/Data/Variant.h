/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <chrono>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include <Spix/spix_export.h>

namespace spix {

struct Variant;

namespace {
using VariantBaseType = std::variant<std::nullptr_t, bool, long long, unsigned long long, double, std::string,
    std::chrono::time_point<std::chrono::system_clock>, std::vector<Variant>, std::map<std::string, Variant>>;
}

/**
 * Utility union type that contains a number of RPC-able types, including a list of itself and a map of {std::string:
 * itself}. Inherits from std::variant. This variant is used to abstract between RPC union types (ex anyrpc::Value) the
 * scene union types (ex. QVariant).
 *
 * NOTE: std::visit is broken for this variant for GCC <= 11.2 and clang <= 14.0, see
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2162r0.html for more info. Instead, type switches must be
 * created manually using the index() method and the TypeIndex enum.
 */
struct SPIX_EXPORT Variant : VariantBaseType {
    using ListType = std::vector<Variant>;
    using MapType = std::map<std::string, Variant>;
    using VariantType = VariantBaseType;
    using VariantBaseType::variant;
    VariantBaseType const& base() const { return *this; }
    VariantBaseType& base() { return *this; }

    enum TypeIndex
    {
        Nullptr = 0,
        Bool,
        Int,
        Uint,
        Double,
        String,
        Time,
        List,
        Map,
        TypeIndexCount
    };
};

static_assert(
    Variant::TypeIndexCount == std::variant_size_v<VariantBaseType>, "Variant enum does not cover all Variant types");

} // namespace spix
