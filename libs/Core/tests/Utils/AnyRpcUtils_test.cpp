/***
 * Copyright (C) Noah Koontz. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <limits>

#include <gtest/gtest.h>

#include <Utils/AnyRpcUtils.h>

using Variant = spix::Variant;
using Value = anyrpc::Value;

TEST(AnyRpcUtilsTest, InvalidValueToVariantThrows)
{
    EXPECT_THROW(spix::utils::AnyRPCValueToVariant(Value()), anyrpc::AnyRpcException);
}

TEST(AnyRpcUtilsTest, BinaryValueToVariantThrows)
{
    Value arg = Value();
    const unsigned char binary[] = {0, 1, 2, 3, 5};
    arg.SetBinary(binary, sizeof(binary));
    EXPECT_THROW(spix::utils::AnyRPCValueToVariant(arg), anyrpc::AnyRpcException);
}

TEST(AnyRpcUtilsTest, NullValueToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(anyrpc::ValueType::NullType));
    EXPECT_EQ(ret, Variant(nullptr));
}

TEST(AnyRpcUtilsTest, BoolValueToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(true));
    EXPECT_EQ(ret, Variant(true));
}

TEST(AnyRpcUtilsTest, StringValueToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value("hello strings!"));
    EXPECT_EQ(ret, Variant(std::string("hello strings!")));
}

TEST(AnyRpcUtilsTest, UnicodeStringValueToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(u8"☀☁☂☃☄★☆☇"));
    EXPECT_EQ(ret, Variant(std::string(u8"☀☁☂☃☄★☆☇")));
}

TEST(AnyRpcUtilsTest, NumberToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(static_cast<int64_t>(-8675309)));
    EXPECT_EQ(ret, Variant(-8675309LL));
}

TEST(AnyRpcUtilsTest, LongNumberToVariant)
{
    int64_t val = std::numeric_limits<long long>::max();
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(val));
    EXPECT_EQ(ret, Variant(static_cast<long long>(val)));
}

TEST(AnyRpcUtilsTest, FloatToVariant)
{
    Variant ret = spix::utils::AnyRPCValueToVariant(Value(2.1));
    EXPECT_EQ(ret, Variant(2.1));
}

TEST(AnyRpcUtilsTest, DateToVariant)
{
    Value arg = Value();
    arg.SetDateTime(1651775070);
    Variant ret = spix::utils::AnyRPCValueToVariant(arg);
    EXPECT_EQ(ret, Variant(std::chrono::system_clock::from_time_t(1651775070)));
}

TEST(AnyRpcUtilsTest, ListToVariant)
{
    Value arg = Value();
    arg.SetArray();

    Value elem0(true);
    Value elem1(137);
    Value elem2("hello strings!");

    arg.PushBack(elem0);
    arg.PushBack(elem1);
    arg.PushBack(elem2);

    Variant ret = spix::utils::AnyRPCValueToVariant(arg);
    EXPECT_EQ(ret.index(), Variant::TypeIndex::List);
    Variant::ListType list = std::get<Variant::ListType>(ret);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], Variant(true));
    EXPECT_EQ(list[1], Variant(137LL));
    EXPECT_EQ(list[2], Variant(std::string("hello strings!")));
}

TEST(AnyRpcUtilsTest, MapToVariant)
{
    Value arg = Value();
    arg.SetMap();

    Value elem0(true);
    Value elem1(137);
    Value elem2("hello strings!");

    arg.AddMember("key0", elem0);
    arg.AddMember("key1", elem1);
    arg.AddMember("key2", elem2);

    Variant ret = spix::utils::AnyRPCValueToVariant(arg);
    EXPECT_EQ(ret.index(), Variant::TypeIndex::Map);
    Variant::MapType map = std::get<Variant::MapType>(ret);
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map["key0"], Variant(true));
    EXPECT_EQ(map["key1"], Variant(137LL));
    EXPECT_EQ(map["key2"], Variant(std::string("hello strings!")));
}

TEST(AnyRpcUtilsTest, MapOfListToVariant)
{
    Value elem0(true);
    Value elem1(137);
    Value elem2("hello strings!");
    Value innerArg = Value();
    innerArg.SetArray();
    innerArg.PushBack(elem0);
    innerArg.PushBack(elem1);
    innerArg.PushBack(elem2);

    Value outerArg = Value();
    outerArg.SetMap();
    outerArg.AddMember("key0", innerArg);

    Variant ret = spix::utils::AnyRPCValueToVariant(outerArg);
    EXPECT_EQ(ret.index(), Variant::TypeIndex::Map);
    Variant::MapType map = std::get<Variant::MapType>(ret);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map["key0"].index(), Variant::TypeIndex::List);

    Variant::ListType list = std::get<Variant::ListType>(map["key0"]);
    EXPECT_EQ(list.size(), 3);
    EXPECT_EQ(list[0], Variant(true));
    EXPECT_EQ(list[1], Variant(137LL));
    EXPECT_EQ(list[2], Variant(std::string("hello strings!")));
}

TEST(AnyRpcUtilsTest, NullToValue)
{
    auto var = Variant(nullptr);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsNull());
}

TEST(AnyRpcUtilsTest, BoolToValue)
{
    auto var = Variant(true);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsBool());
    EXPECT_TRUE(val.GetBool());
}

TEST(AnyRpcUtilsTest, NumberToValue)
{
    auto var = Variant(-137LL);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsInt64());
    EXPECT_EQ(val.GetInt64(), -137);
}

TEST(AnyRpcUtilsTest, LongNumberToValue)
{
    long long number = std::numeric_limits<long long>::max();
    auto var = Variant(number);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsInt64());
    EXPECT_EQ(val.GetInt64(), number);
}

TEST(AnyRpcUtilsTest, FloatToValue)
{
    auto var = Variant(2.1);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsFloat());
    EXPECT_EQ(val.GetDouble(), 2.1);
}

TEST(AnyRpcUtilsTest, StringToValue)
{
    auto var = Variant(std::string("hello strings!"));
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsString());
    EXPECT_STREQ(val.GetString(), "hello strings!");
}

TEST(AnyRpcUtilsTest, UnicodeToValue)
{
    auto var = Variant(std::string(u8"☀☁☂☃☄★☆☇"));
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsString());
    EXPECT_STREQ(val.GetString(), u8"☀☁☂☃☄★☆☇");
}

TEST(AnyRpcUtilsTest, DateToValue)
{
    auto var = Variant(std::chrono::system_clock::from_time_t(1651775070));
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsDateTime());
    EXPECT_EQ(val.GetDateTime(), 1651775070);
}

TEST(AnyRpcUtilsTest, ListToValue)
{
    auto list = Variant::ListType {Variant(true), Variant(137LL), Variant(std::string("hello strings!"))};
    auto var = Variant(list);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsArray());
    EXPECT_EQ(val.Size(), 3);
    EXPECT_TRUE(val[0].IsBool());
    EXPECT_EQ(val[0].GetBool(), true);
    EXPECT_TRUE(val[1].IsInt64());
    EXPECT_EQ(val[1].GetInt64(), 137);
    EXPECT_TRUE(val[2].IsString());
    EXPECT_STREQ(val[2].GetString(), "hello strings!");
}

TEST(AnyRpcUtilsTest, MapToValue)
{
    auto map = Variant::MapType {
        {"key0", Variant(true)}, {"key1", Variant(137LL)}, {"key2", Variant(std::string("hello strings!"))}};
    auto var = Variant(map);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsMap());
    EXPECT_EQ(val.MemberCount(), 3);
    EXPECT_TRUE(val["key0"].IsBool());
    EXPECT_EQ(val["key0"].GetBool(), true);
    EXPECT_TRUE(val["key1"].IsInt64());
    EXPECT_EQ(val["key1"].GetInt64(), 137);
    EXPECT_TRUE(val["key2"].IsString());
    EXPECT_STREQ(val["key2"].GetString(), "hello strings!");
}

TEST(AnyRpcUtilsTest, MapOfListToValue)
{
    auto map = Variant::MapType {
        {"key0", Variant::ListType {Variant(true), Variant(137LL), Variant(std::string("hello strings!"))}}};
    auto var = Variant(map);
    auto val = spix::utils::VariantToAnyRPCValue(var);

    EXPECT_TRUE(val.IsMap());
    EXPECT_EQ(val.MemberCount(), 1);
    auto innerList = val["key0"];
    EXPECT_TRUE(innerList.IsArray());
    EXPECT_EQ(innerList.Size(), 3);
    EXPECT_TRUE(innerList[0].IsBool());
    EXPECT_EQ(innerList[0].GetBool(), true);
    EXPECT_TRUE(innerList[1].IsInt64());
    EXPECT_EQ(innerList[1].GetInt64(), 137);
    EXPECT_TRUE(innerList[2].IsString());
    EXPECT_STREQ(innerList[2].GetString(), "hello strings!");
}
