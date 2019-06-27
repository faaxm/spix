/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <CommandExecuter/CommandExecuter.h>
#include <Commands/ClickOnItem.h>
#include <Commands/CustomCmd.h>
#include <Scene/Mock/MockScene.h>

TEST(ClickOnItemTest, ErrorOnMissingItem)
{
    auto command = std::make_unique<spix::cmd::ClickOnItem>(spix::ItemPosition("window/some/item"));

    spix::MockScene scene;
    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));

    // Try a couple of times to process all commands
    exec.processCommands(scene);
    exec.processCommands(scene);
    exec.processCommands(scene);

    // Check that an error is available
    EXPECT_TRUE(exec.state().hasErrors());
}

TEST(ClickOnItemTest, ClickItem)
{
    bool didPostClickEvent = false;
    bool mouseDown = false;

    auto command = std::make_unique<spix::cmd::ClickOnItem>(spix::ItemPosition("window/some/item"));

    spix::MockScene scene;
    scene.addItemAtPath(spix::Size(100.0, 30.0), "window/some/item");

    scene.mockEvents().onMouseClickEvent
        = [&didPostClickEvent, &mouseDown](spix::Item*, spix::Point loc, bool press, bool release) {
              // Press and release the mouse button
              EXPECT_TRUE(mouseDown || press);
              EXPECT_TRUE(!mouseDown || release);
              // With the mouse in the center of the item
              EXPECT_TRUE(loc.x > 49.0 && loc.x < 51.0);
              EXPECT_TRUE(loc.y > 14.0 && loc.y < 16.0);
              // Been here, done that.
              didPostClickEvent = true;

              if (press)
                  mouseDown = true;
              else
                  mouseDown = false;
          };

    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));
    exec.processCommands(scene);

    // a click event was triggered
    EXPECT_TRUE(didPostClickEvent);
}
