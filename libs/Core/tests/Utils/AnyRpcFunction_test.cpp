/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Utils/AnyRpcFunction.h>
#include <anyrpc/anyrpc.h>

TEST(AnyRpcFunctionTest, ThreeArgsNoReturn)
{
    anyrpc::MethodManager manager;

    int res_arg_a;
    int res_arg_b;
    std::string res_arg_c;

    spix::utils::AddFunctionToAnyRpc<void(int, int, std::string)>(
        &manager, "test_func", "Help Text", [&](int a, int b, std::string c) {
            res_arg_a = a;
            res_arg_b = b;
            res_arg_c = c;
        });

    anyrpc::Value result;

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();
    args[0] = anyrpc::Value(6234);
    args[1] = anyrpc::Value(24615);
    args[2] = anyrpc::Value("Some String");

    // Call function
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_EQ(res_arg_a, 6234);
    EXPECT_EQ(res_arg_b, 24615);
    EXPECT_EQ(res_arg_c, "Some String");
}

TEST(AnyRpcFunctionTest, VecArgsNoReturn)
{
    anyrpc::MethodManager manager;

    std::string res_arg_a;
    std::string res_arg_b;

    spix::utils::AddFunctionToAnyRpc<void(std::vector<std::string>)>(
        &manager, "test_func", "Help Text", [&](std::vector<std::string> vec) {
            res_arg_a = vec[0];
            res_arg_b = vec[1];
        });

    // Construct vector argument
    anyrpc::Value inputVect;
    inputVect.SetArray();
    inputVect[0] = anyrpc::Value("Hello");
    inputVect[1] = anyrpc::Value("World");

    anyrpc::Value result;

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();
    args[0] = inputVect;

    // Call function
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_EQ(res_arg_a, "Hello");
    EXPECT_EQ(res_arg_b, "World");
}

TEST(AnyRpcFunctionTest, NoArgWithReturn)
{
    anyrpc::MethodManager manager;

    spix::utils::AddFunctionToAnyRpc<int()>(&manager, "test_func", "Help Text", [&]() { return 53562; });

    anyrpc::Value result;

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();

    // Call function
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_EQ(result.GetInt(), 53562);
}

TEST(AnyRpcFunctionTest, NoArgWithVecReturn)
{
    anyrpc::MethodManager manager;

    spix::utils::AddFunctionToAnyRpc<std::vector<std::string>()>(
        &manager, "test_func", "Help Text", [&]() -> std::vector<std::string> {
            return {"Hi", "there"};
        });

    anyrpc::Value result;

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();

    // Call function
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_TRUE(result.IsArray());
    EXPECT_EQ(result[0].GetString(), std::string("Hi"));
    EXPECT_EQ(result[1].GetString(), std::string("there"));
}

TEST(AnyRpcFunctionTest, VarVecArgNoReturn)
{
    using Variant = spix::Variant;

    anyrpc::MethodManager manager;

    Variant res_arg_a;
    Variant res_arg_b;

    spix::utils::AddFunctionToAnyRpc<void(std::vector<Variant>)>(
        &manager, "test_func", "Help Text", [&](std::vector<Variant> vec) {
            res_arg_a = vec[0];
            res_arg_b = vec[1];
        });

    // Construct vector argument
    anyrpc::Value inputVect;
    inputVect.SetArray();
    inputVect[0] = anyrpc::Value("Hello");
    inputVect[1] = anyrpc::Value("World");

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();
    args[0] = inputVect;

    // Call function
    anyrpc::Value result;
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_EQ(res_arg_a, Variant(std::string("Hello")));
    EXPECT_EQ(res_arg_b, Variant(std::string("World")));
}

TEST(AnyRpcFunctionTest, NoArgWithVecVarReturn)
{
    using Variant = spix::Variant;

    anyrpc::MethodManager manager;

    spix::utils::AddFunctionToAnyRpc<Variant()>(
        &manager, "test_func", "Help Text", [&]() -> Variant { return Variant(std::string("Hi there")); });

    anyrpc::Value result;

    // Construct array with the functions arguments
    anyrpc::Value args;
    args.SetArray();

    // Call function
    manager.ExecuteMethod("test_func", args, result);

    // Check
    EXPECT_TRUE(result.IsString());
    EXPECT_EQ(result.GetString(), std::string("Hi there"));
}
