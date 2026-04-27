#include <gtest/gtest.h>

#include <Commands/DumpTree.h>
#include <Scene/Mock/MockScene.h>
#include <Spix/CommandExecuter/CommandExecuter.h>

TEST(DumpTreeCommandTest, ReturnsSceneResult)
{
    std::promise<std::string> promise;
    auto result = promise.get_future();
    auto command = std::make_unique<spix::cmd::DumpTree>("window", std::move(promise));

    spix::MockScene scene;
    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));
    exec.processCommands(scene);

    // MockScene::dumpTree returns "{}"
    EXPECT_EQ(result.get(), "{}");
}

TEST(DumpTreeCommandTest, NoErrorsOnExecution)
{
    std::promise<std::string> promise;
    auto result = promise.get_future();
    auto command = std::make_unique<spix::cmd::DumpTree>("window/item", std::move(promise));

    spix::MockScene scene;
    spix::CommandExecuter exec;
    exec.enqueueCommand(std::move(command));
    exec.processCommands(scene);

    result.get(); // consume the future
    EXPECT_FALSE(exec.state().hasErrors());
}
