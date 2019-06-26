/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <CommandExecuter/ExecuterState.h>

TEST(ExecuterStateTest, Plain)
{
    spix::ExecuterState state;

    EXPECT_FALSE(state.hasErrors());

    state.reportError("Some Error");
    EXPECT_TRUE(state.hasErrors());

    EXPECT_EQ(state.errorsDescription(), "Some Error");
}
