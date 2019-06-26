/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <CommandExecuter/CommandExecuter.h>
#include <Commands/CustomCmd.h>
#include <Commands/DropFromExt.h>
#include <Scene/Mock/MockScene.h>

TEST(DropFromExtTest, ErrorOnMissingItem)
{
    auto command = std::make_unique<spix::cmd::DropFromExt>(
        "window/some/item", spix::makePasteboardContentWithUrls({"file://"}));

    spix::MockScene scene;
    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));

    // Process command
    exec.processCommands(scene);

    // Check that an error is available
    EXPECT_TRUE(exec.state().hasErrors());
}

TEST(DropFromExtTest, DropData)
{
    bool didPostClickEvent = false;

    auto command = std::make_unique<spix::cmd::DropFromExt>(
        "window/some/item", spix::makePasteboardContentWithUrls({"file://some/file"}));

    spix::MockScene scene;
    scene.addItemAtPath(spix::Size(100.0, 30.0), "window/some/item");

    scene.mockEvents().onMouseDropEvent
        = [&didPostClickEvent](spix::Item*, spix::Point loc, spix::PasteboardContent& data) {
              // With the mouse in the center of the item
              EXPECT_TRUE(loc.x > 49.0 && loc.x < 51.0);
              EXPECT_TRUE(loc.y > 14.0 && loc.y < 16.0);
              // Right data
              EXPECT_TRUE(data.hasUrls());
              EXPECT_EQ(data.urls().at(0), "file://some/file");
              // Been here, done that.
              didPostClickEvent = true;
          };

    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));
    exec.processCommands(scene);

    // a click event was triggered
    EXPECT_TRUE(didPostClickEvent);
}
