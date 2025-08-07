/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Spix/CommandExecuter/CommandExecuter.h>
#include <Commands/GetProperty.h>
#include <Scene/Mock/MockScene.h>

TEST(GetPropertyTest, GetStringProperty)
{
    // Build command
    std::promise<std::string> promise;
    auto result = promise.get_future();
    auto command = std::make_unique<spix::cmd::GetProperty>("window/some/item", "testProperty", std::move(promise));

    // Build Scene with Item
    spix::MockScene scene;
    spix::MockItem item {spix::Size(100.0, 30.0)};
    item.stringProperties()["testProperty"] = "This is a test value";
    scene.addItemAtPath(std::move(item), "window/some/item");

    // Execute Commands
    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));
    exec.processCommands(scene);

    // Check Outcome
    EXPECT_EQ(result.get(), "This is a test value");
}
