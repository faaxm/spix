/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "AnyRpcUtils.h"

#include <stdexcept>

namespace spix {
namespace utils {

Variant AnyRPCValueToVariant(const anyrpc::Value& valueConst)
{
    anyrpc::Value value(valueConst);

    switch (value.GetType()) {
    case anyrpc::ValueType::FalseType:
        return Variant(false);
    case anyrpc::ValueType::TrueType:
        return Variant(true);
    case anyrpc::ValueType::StringType:
        return Variant(std::string(value.GetString()));
    case anyrpc::ValueType::NumberType:
        if (value.IsDouble() || value.IsFloat())
            return Variant(value.GetDouble());
        if (value.IsInt() || value.IsInt64())
            return Variant(static_cast<long long>(value.GetInt64()));
        if (value.IsUint() || value.IsUint64())
            return Variant(static_cast<unsigned long long>(value.GetUint64()));
        else
            return Variant(nullptr);
    case anyrpc::ValueType::DateTimeType: {
        const std::time_t time = value.GetDateTime();
        return Variant(std::chrono::system_clock::from_time_t(time));
    }
    case anyrpc::ValueType::ArrayType: {
        Variant::ListType list;
        for (std::size_t i = 0; i < value.Size(); i++)
            list.push_back(AnyRPCValueToVariant(value[i]));
        return Variant(list);
    }
    case anyrpc::ValueType::MapType: {
        Variant::MapType map;
        for (auto ptr = value.MemberBegin(); ptr != value.MemberEnd(); ptr++) {
            anyrpc::Value key = ptr.GetKey();
            if (!key.IsString())
                throw anyrpc::AnyRpcException(
                    anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters: dict keys must be a string.");
            map[key.GetString()] = AnyRPCValueToVariant(ptr.GetValue());
        }
        return Variant(map);
    }
    case anyrpc::ValueType::NullType:
        return Variant(nullptr);
    case anyrpc::ValueType::BinaryType:
    case anyrpc::ValueType::InvalidType:
    default:
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters: unknown parameter type");
    }
}

anyrpc::Value VariantToAnyRPCValue(const Variant& value)
{
    // missing std::visit here :'(
    static_assert(Variant::TypeIndexCount == 9, "VariantToAnyRPCValue does not cover all Variant types");
    switch (value.index()) {
    case Variant::Nullptr:
        return anyrpc::Value(anyrpc::NullType);
    case Variant::Bool:
        return anyrpc::Value(std::get<bool>(value));
    case Variant::Int:
        return anyrpc::Value(static_cast<int64_t>(std::get<long long>(value)));
    case Variant::Uint:
        return anyrpc::Value(static_cast<uint64_t>(std::get<unsigned long long>(value)));
    case Variant::Double:
        return anyrpc::Value(std::get<double>(value));
    case Variant::String:
        return anyrpc::Value(std::get<std::string>(value));
    case Variant::Time: {
        auto time = std::get<std::chrono::time_point<std::chrono::system_clock>>(value);
        std::time_t timet = std::chrono::system_clock::to_time_t(time);
        anyrpc::Value ret;
        ret.SetDateTime(timet);
        return ret;
    }
    case Variant::List: {
        anyrpc::Value ret;
        ret.SetArray();
        for (const auto& elem : std::get<Variant::ListType>(value)) {
            anyrpc::Value val = VariantToAnyRPCValue(elem);
            ret.PushBack(val);
        }
        return ret;
    }
    case Variant::Map: {
        anyrpc::Value ret;
        ret.SetMap();
        for (const auto& [key, value] : std::get<Variant::MapType>(value)) {
            anyrpc::Value temp = VariantToAnyRPCValue(value);
            ret.AddMember(key, temp);
        }
        return ret;
    }
    default:
        throw std::runtime_error("VariantToAnyRPCValue received Variant with unknown type");
    }
}

} // namespace utils
} // namespace spix
