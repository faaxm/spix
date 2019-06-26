/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <CommandExecuter/CommandExecuter.h>
#include <Commands/Command.h>
#include <Commands/CustomCmd.h>
#include <Scene/Mock/MockScene.h>

TEST(CommandExecuterTest, Plain)
{
    spix::CommandExecuter exec;
    spix::MockScene scene;

    int didExec1 = 0;
    int didExec2 = 0;
    int didExec3 = 0;

    // Running an empty CommandExecuter shouldn't break stuff
    exec.processCommands(scene);
    // Enqueue test commands
    exec.enqueueCommand(
        std::make_unique<spix::cmd::CustomCmd>([&](spix::CommandEnvironment&) { ++didExec1; }, [] { return true; }));
    exec.enqueueCommand(
        std::make_unique<spix::cmd::CustomCmd>([&](spix::CommandEnvironment&) { ++didExec2; }, [] { return true; }));
    exec.enqueueCommand(
        std::make_unique<spix::cmd::CustomCmd>([&](spix::CommandEnvironment&) { ++didExec3; }, [] { return true; }));

    // process. Should execute every command once
    exec.processCommands(scene);
    EXPECT_EQ(didExec1, 1);
    EXPECT_EQ(didExec2, 1);
    EXPECT_EQ(didExec3, 1);

    // processing again shouldn't change anything
    exec.processCommands(scene);
    EXPECT_EQ(didExec1, 1);
    EXPECT_EQ(didExec2, 1);
    EXPECT_EQ(didExec3, 1);
}

TEST(CommandExecuterTest, Blocked)
{
    spix::CommandExecuter exec;
    spix::MockScene scene;

    // Control / Test vars
    int didExec1 = 0;
    int didExec2 = 0;
    int didExec3 = 0;
    bool canExec2 = false;

    // Add commands to executer and execute
    exec.enqueueCommand(
        std::make_unique<spix::cmd::CustomCmd>([&](spix::CommandEnvironment&) { ++didExec1; }, [] { return true; }));
    exec.enqueueCommand(std::make_unique<spix::cmd::CustomCmd>(
        [&](spix::CommandEnvironment&) { ++didExec2; }, [&] { return canExec2; }));
    exec.enqueueCommand(
        std::make_unique<spix::cmd::CustomCmd>([&](spix::CommandEnvironment&) { ++didExec3; }, [] { return true; }));
    exec.processCommands(scene);

    // Only the first command should have been executed
    EXPECT_EQ(didExec1, 1);
    EXPECT_EQ(didExec2, 0);
    EXPECT_EQ(didExec3, 0);

    // processing commands again shouldn't change anything
    exec.processCommands(scene);
    EXPECT_EQ(didExec1, 1);
    EXPECT_EQ(didExec2, 0);
    EXPECT_EQ(didExec3, 0);

    // We unblock the 2nd command, now all should have been executed
    canExec2 = true;
    exec.processCommands(scene);
    EXPECT_EQ(didExec1, 1);
    EXPECT_EQ(didExec2, 1);
    EXPECT_EQ(didExec3, 1);
}

TEST(CommandExecuterTest, NoBreakOnError)
{
    bool didExec2ndCommand = false;

    spix::CommandExecuter exec;
    spix::MockScene scene;

    // Add commands to executer and execute
    exec.enqueueCommand(std::make_unique<spix::cmd::CustomCmd>(
        [&](spix::CommandEnvironment& env) { env.state().reportError("Some Error Happened"); }, [] { return true; }));
    exec.enqueueCommand(std::make_unique<spix::cmd::CustomCmd>(
        [&](spix::CommandEnvironment&) { didExec2ndCommand = true; }, [] { return true; }));

    exec.processCommands(scene);
    exec.processCommands(scene);
    exec.processCommands(scene);
    exec.processCommands(scene);

    EXPECT_TRUE(didExec2ndCommand);
    EXPECT_EQ(exec.state().errorsDescription(), "Some Error Happened");
}
