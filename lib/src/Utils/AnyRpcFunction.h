/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Data/Variant.h>
#include <Utils/AnyRpcUtils.h>
#include <anyrpc/anyrpc.h>
#include <functional>

/**
 * Utility type traits
 */

namespace {
template <class T, template <class...> class Template>
struct is_specialization : std::false_type {
};

template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {
};
} // namespace

/**
 * The templates in this file make it easier to register functions with AnyRPC.
 * It is now sufficient to register a function. The parameter parsing will automatically
 * be adapted to the signature of the function, including type checks and conversion.
 */

namespace spix {
namespace utils {
/**
 * Tools to convert an anyrpc::Value to a
 * particular type. If the requested type does not
 * match the content of the anyrpc::Value, an excption
 * is thrown.
 **/
template <typename T>
T unpackAnyRpcParam(anyrpc::Value&);

template <>
Variant unpackAnyRpcParam(anyrpc::Value& value)
{
    return AnyRPCValueToVariant(value);
}

template <>
std::vector<Variant> unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsArray()) {
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Array.");
    }
    std::vector<Variant> result;
    for (size_t i = 0; i < value.Size(); i++)
        result.push_back(AnyRPCValueToVariant(value[i]));
    return result;
}

template <>
int unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsNumber()) {
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Int.");
    }
    return value.GetInt();
}

template <>
unsigned unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsUint()) {
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Int.");
    }
    return value.GetUint();
}

template <>
std::string unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsString()) {
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected String.");
    }
    return value.GetString();
}

template <>
std::vector<std::string> unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsArray()) {
        throw anyrpc::AnyRpcException(
            anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Array of Strings.");
    }
    std::vector<std::string> result;
    result.reserve(value.Size());
    for (size_t i = 0; i < value.Size(); ++i) {
        if (!value[i].IsString()) {
            throw anyrpc::AnyRpcException(
                anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Array of Strings.");
        }
        result.push_back(value[i].GetString());
    }

    return result;
}

/**
 * Functions that call a std::function and assign its returned
 * value to the given anyrpc::Value. If the return type
 * of the std::function is 'void', no value is assigned.
 */

template <typename R, typename... Args>
void callAndAssignAnyRpcResult(std::function<R(Args...)> func, anyrpc::Value& result, Args... args)
{
    if constexpr (std::is_void_v<R>) {
        func(std::forward<Args>(args)...);
    } else {
        R funcResult = func(std::forward<Args>(args)...);

        if constexpr (is_specialization<R, std::vector>::value) {
            result.SetArray();
            for (const auto& item : funcResult) {
                anyrpc::Value value {item};
                result.PushBack(value);
            }
        } else if constexpr (std::is_same_v<R, Variant>) {
            result = VariantToAnyRPCValue(funcResult);
        } else {
            result = funcResult;
        }
    }
}

template <typename... Args, std::size_t... Is, typename F>
void unpackCallAndAssignAnyRpcResult(F func, anyrpc::Value& result, std::index_sequence<Is...>, anyrpc::Value& params)
{
    callAndAssignAnyRpcResult(func, result, unpackAnyRpcParam<Args>(params[Is])...);
}

/**
 * The AnyRpcFunction object that takes a std::function that is
 * to be called. The parameters received from AnyRPC are automatically
 * converted and type checked to the correct type based on the function signature.
 **/
template <typename F>
class AnyRpcFunction;

template <typename R, typename... Args>
class AnyRpcFunction<R(Args...)> : public anyrpc::Method {
public:
    AnyRpcFunction(
        std::function<R(Args...)> func, const std::string& name, const std::string& help, bool deleteOnRemove = true)
    : anyrpc::Method(name, help, deleteOnRemove)
    , m_func(func)
    {
    }

    void Execute(anyrpc::Value& params, anyrpc::Value& result) override
    {
        if (!params.IsArray()) {
            throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Array.");
        }
        if (params.Size() != sizeof...(Args)) {
            throw anyrpc::AnyRpcException(
                anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Number of parameters incorrect.");
        }

        unpackCallAndAssignAnyRpcResult<Args...>(m_func, result, std::make_index_sequence<sizeof...(Args)>(), params);
    }

private:
    std::function<R(Args...)> m_func;
};

/**
 * Helper function to add a std::function based function to AnyRPC.
 **/
template <typename F>
void AddFunctionToAnyRpc(
    anyrpc::MethodManager* manager, const std::string& name, const std::string& help, std::function<F> func)
{
    manager->AddMethod(new utils::AnyRpcFunction<F>(func, name, help, true));
}

} // namespace utils
} // namespace spix
