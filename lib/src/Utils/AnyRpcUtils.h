/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <anyrpc/anyrpc.h>

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
int unpackAnyRpcParam(anyrpc::Value& value)
{
    if (!value.IsNumber()) {
        throw anyrpc::AnyRpcException(anyrpc::AnyRpcErrorInvalidParams, "Invalid parameters. Expected Int.");
    }
    return value.GetInt();
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
 * Converts a value in an anyrpc::Value array at the given index
 * to the requested type and increases the index.
 **/
template <typename T>
T unpackAnyRpcParamArray(anyrpc::Value& params, int& index)
{
    auto value = unpackAnyRpcParam<T>(params[index]);
    ++index;
    return value;
}

/**
 * Functions that call a std::function and assign its returned
 * value to the given anyrpc::Value. If the return type
 * of the std::function is 'void', no value is assigned.
 */
template <typename R, typename = std::enable_if_t<!std::is_void<R>::value>, typename... Args>
void callAndAssignAnyRpcResult(std::function<R(Args...)> func, anyrpc::Value& result, Args... args)
{
    result = func(std::forward<Args>(args)...);
}

template <typename R,typename... Args>
void callAndAssignAnyRpcResult(std::function<std::vector<R>(Args...)> func, anyrpc::Value& result, Args... args)
{
    auto funcResult = func(std::forward<Args>(args)...);

    result.SetArray();
    for (const auto& item : funcResult) {
        anyrpc::Value value {item};
        result.PushBack(value);
    }
}

template <typename... Args>
void callAndAssignAnyRpcResult(std::function<void(Args...)> func, anyrpc::Value& /*result*/, Args... args)
{
    func(std::forward<Args>(args)...);
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

        int paramIndex = 0;
        callAndAssignAnyRpcResult(m_func, result, unpackAnyRpcParamArray<Args>(params, paramIndex)...);
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
