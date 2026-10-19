/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <gtest/gtest.h>

#include <Scene/Mock/MockScene.h>
#include <Spix/CommandExecuter/CommandExecuter.h>
#include <Spix/TestServer.h>

namespace {

class TestServer : public spix::TestServer {
    void executeTest() override {}
};

class RecordingScene : public spix::MockScene {
public:
    std::unique_ptr<spix::Item> itemAtPath(const spix::ItemPath& path) override
    {
        ++lookups;
        return MockScene::itemAtPath(path);
    }

    int lookups = 0;
};

// A backend that implements only the API available before double-click support.
class LegacyEvents : public spix::Events {
public:
    void mouseDown(spix::Item*, spix::Point, spix::MouseButton, spix::KeyModifier) override { ++mouseEvents; }
    void mouseUp(spix::Item*, spix::Point, spix::MouseButton, spix::KeyModifier) override { ++mouseEvents; }
    void mouseMove(spix::Item*, spix::Point) override { ++mouseEvents; }
    void stringInput(spix::Item*, const std::string&) override {}
    void keyPress(spix::Item*, int, spix::KeyModifier) override {}
    void keyRelease(spix::Item*, int, spix::KeyModifier) override {}
    void extMouseDrop(spix::Item*, spix::Point, spix::PasteboardContent&) override {}
    void quit() override {}

    int mouseEvents = 0;
};

class LegacyScene : public RecordingScene {
public:
    spix::Events& events() override { return legacyEvents; }
    LegacyEvents legacyEvents;
};

struct Gesture {
    spix::Point position;
    spix::MouseButton button;
    spix::KeyModifier modifiers;
};

class DoubleClickOnItemTest : public ::testing::Test {
protected:
    DoubleClickOnItemTest()
    {
        server.setCommandExecuter(&exec);
        scene.addItemAtPath(spix::Size(100.0, 40.0), "window/target");
        scene.mockEvents().onMouseDoubleClickEvent
            = [this](spix::Item* item, spix::Point point, spix::MouseButton button, spix::KeyModifier modifiers) {
                  EXPECT_NE(item, nullptr);
                  gestures.push_back({point, button, modifiers});
                  return true;
              };
        scene.mockEvents().onMouseClickEvent = [this](spix::Item*, spix::Point, bool, bool) { ++singleClickEvents; };
    }

    RecordingScene scene;
    spix::CommandExecuter exec;
    TestServer server;
    std::vector<Gesture> gestures;
    int singleClickEvents = 0;
};

TEST_F(DoubleClickOnItemTest, PublicOverloadsQueueResolvedPositions)
{
    server.mouseDoubleClick("window/target");
    server.mouseDoubleClick("window/target", spix::Point(0.25, 0.75));
    server.mouseDoubleClick("window/target", spix::Point(0.25, 0.75), spix::Point(3.0, -2.0));
    server.mouseDoubleClick("window/target", spix::MouseButtons::Right);
    EXPECT_TRUE(gestures.empty());

    exec.processCommands(scene);

    ASSERT_EQ(gestures.size(), 4u);
    const spix::Point expectedPositions[] = {{50.0, 20.0}, {25.0, 30.0}, {28.0, 28.0}, {50.0, 20.0}};
    for (size_t i = 0; i < gestures.size(); ++i) {
        EXPECT_DOUBLE_EQ(gestures[i].position.x, expectedPositions[i].x);
        EXPECT_DOUBLE_EQ(gestures[i].position.y, expectedPositions[i].y);
        EXPECT_EQ(gestures[i].button, i == 3 ? spix::MouseButtons::Right : spix::MouseButtons::Left);
        EXPECT_EQ(gestures[i].modifiers, spix::KeyModifiers::None);
    }
    EXPECT_FALSE(exec.state().hasErrors());
    EXPECT_EQ(singleClickEvents, 0);
}

TEST_F(DoubleClickOnItemTest, ForwardsAllSupportedButtonsAndModifierCombinations)
{
    for (auto button : {spix::MouseButtons::Left, spix::MouseButtons::Right, spix::MouseButtons::Middle}) {
        for (spix::KeyModifier modifiers = 0; modifiers < 16; ++modifiers)
            server.mouseDoubleClick("window/target", button, modifiers);
    }
    exec.processCommands(scene);

    ASSERT_EQ(gestures.size(), 48u);
    size_t i = 0;
    for (auto button : {spix::MouseButtons::Left, spix::MouseButtons::Right, spix::MouseButtons::Middle}) {
        for (spix::KeyModifier modifiers = 0; modifiers < 16; ++modifiers, ++i) {
            EXPECT_EQ(gestures[i].button, button);
            EXPECT_EQ(gestures[i].modifiers, modifiers);
        }
    }
    EXPECT_FALSE(exec.state().hasErrors());
}

TEST_F(DoubleClickOnItemTest, RejectsInvalidButtonsBeforeItemLookup)
{
    for (spix::MouseButton button : {0u, 3u, 5u, 6u, 7u, 8u, 9u})
        server.mouseDoubleClick("window/target", button);
    exec.processCommands(scene);

    EXPECT_EQ(exec.state().errors().size(), 7u);
    EXPECT_EQ(scene.lookups, 0);
    EXPECT_TRUE(gestures.empty());
    EXPECT_EQ(singleClickEvents, 0);
}

TEST_F(DoubleClickOnItemTest, RejectsInvalidModifiersBeforeItemLookup)
{
    server.mouseDoubleClick("window/target", spix::MouseButtons::Left, 16u);
    server.mouseDoubleClick("window/target", spix::MouseButtons::Left, 16u | spix::KeyModifiers::Shift);
    exec.processCommands(scene);

    EXPECT_EQ(exec.state().errors().size(), 2u);
    EXPECT_EQ(scene.lookups, 0);
    EXPECT_TRUE(gestures.empty());
    EXPECT_EQ(singleClickEvents, 0);
}

TEST_F(DoubleClickOnItemTest, MissingItemReportsErrorWithoutEvents)
{
    server.mouseDoubleClick("window/missing");
    exec.processCommands(scene);

    ASSERT_EQ(exec.state().errors().size(), 1u);
    EXPECT_EQ(exec.state().errors()[0], "DoubleClickOnItem: Item not found: window/missing");
    EXPECT_TRUE(gestures.empty());
    EXPECT_EQ(singleClickEvents, 0);
}

TEST_F(DoubleClickOnItemTest, BackendRejectionReportsErrorAndQueueContinues)
{
    scene.mockEvents().onMouseDoubleClickEvent
        = [](spix::Item*, spix::Point, spix::MouseButton, spix::KeyModifier) { return false; };
    server.mouseDoubleClick("window/target");
    server.mouseClick("window/target");
    exec.processCommands(scene);

    ASSERT_EQ(exec.state().errors().size(), 1u);
    EXPECT_EQ(exec.state().errors()[0], "DoubleClickOnItem: Backend could not queue the gesture: window/target");
    EXPECT_EQ(singleClickEvents, 2);
}

TEST_F(DoubleClickOnItemTest, OrdinaryClickDoesNotCallDoubleClickBackend)
{
    server.mouseClick("window/target");
    exec.processCommands(scene);

    EXPECT_TRUE(gestures.empty());
    EXPECT_EQ(singleClickEvents, 2);
    EXPECT_FALSE(exec.state().hasErrors());
}

TEST(DoubleClickOnLegacyBackendTest, ReportsUnsupportedWithoutPartialGesture)
{
    LegacyScene scene;
    scene.addItemAtPath(spix::Size(100.0, 40.0), "window/target");
    spix::CommandExecuter exec;
    TestServer server;
    server.setCommandExecuter(&exec);

    server.mouseDoubleClick("window/target");
    exec.processCommands(scene);

    EXPECT_TRUE(exec.state().hasErrors());
    EXPECT_EQ(scene.legacyEvents.mouseEvents, 0);
}

} // namespace
