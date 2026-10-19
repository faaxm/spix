/***
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <QtItem.h>
#include <QtScene.h>
#include <Spix/CommandExecuter/CommandExecuter.h>
#include <Spix/TestServer.h>

#include <QGuiApplication>
#include <QMouseEvent>
#include <QPointer>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QtTest/QTest>
#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <vector>

namespace {

class Server : public spix::TestServer {
    void executeTest() override {}
};

struct MouseEventRecord {
    QEvent::Type type;
    QPointF position;
    QPointF globalPosition;
    Qt::MouseButton button;
    Qt::MouseButtons buttons;
    Qt::KeyboardModifiers modifiers;
};

class EventRecorder : public QObject {
public:
    std::vector<MouseEventRecord> events;

    bool eventFilter(QObject*, QEvent* event) override
    {
        if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease
            || event->type() == QEvent::MouseButtonDblClick) {
            auto* mouse = static_cast<QMouseEvent*>(event);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            auto position = mouse->position();
            auto globalPosition = mouse->globalPosition();
#else
            auto position = mouse->localPos();
            auto globalPosition = mouse->screenPos();
#endif
            events.push_back(
                {event->type(), position, globalPosition, mouse->button(), mouse->buttons(), mouse->modifiers()});
        }
        return false;
    }
};

class DoubleClickGesture : public testing::Test {
protected:
    void SetUp() override
    {
        QQmlComponent component(&engine);
        component.setData(R"qml(
import QtQuick 2.15
import QtQuick.Window 2.15
Window {
    id: root
    objectName: "gestureWindow"
    width: 520; height: 360
    property int clicks: 0
    property int doubles: 0
    property int keys: 0
    property int tapDoubles: 0
    property string selection: ""
    property string selectedAtDouble: ""
    property real lastX: 0
    property real lastY: 0
    property int lastButton: 0
    property int lastModifiers: 0
    property bool removeOnDouble: false
    property bool acceptDouble: true
    Rectangle {
        objectName: "tapTarget"
        x: 360; y: 250; width: 100; height: 70
        TapHandler {
            objectName: "tapHandler"
            onDoubleTapped: root.tapDoubles++
        }
    }
    Item {
        objectName: "group"
        x: 90; y: 70; width: 280; height: 180
        Loader {
            id: rowLoader
            objectName: "rowLoader"
            x: 30; y: 20
            sourceComponent: Component {
                Rectangle {
                    id: tile
                    width: 160; height: 80; color: "steelblue"
                    MouseArea {
                        id: area
                        objectName: "row"
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
                        drag.target: tile
                        onClicked: { root.clicks++; root.selection = "row" }
                        onDoubleClicked: function(mouse) {
                            root.doubles++
                            root.selectedAtDouble = root.selection
                            root.lastX = mouse.x; root.lastY = mouse.y
                            root.lastButton = mouse.button; root.lastModifiers = mouse.modifiers
                            mouse.accepted = root.acceptDouble
                            if (root.removeOnDouble) rowLoader.active = false
                        }
                        Keys.onPressed: { root.keys++; event.accepted = true }
                    }
                }
            }
        }
    }
}
)qml",
            QUrl("qrc:/double-click-fixture.qml"));
        ASSERT_FALSE(component.isError()) << component.errorString().toStdString();
        window.reset(qobject_cast<QQuickWindow*>(component.create()));
        ASSERT_NE(window, nullptr) << component.errorString().toStdString();
        window->installEventFilter(&recorder);
        window->show();
        ASSERT_TRUE(QTest::qWaitForWindowExposed(window.get(), 5000));
        ASSERT_NE(row(), nullptr);
        server.setCommandExecuter(&executor);
    }

    void TearDown() override
    {
        window.reset();
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
    }

    QQuickItem* row() { return window->findChild<QQuickItem*>("row"); }
    int count(const char* name) { return window->property(name).toInt(); }
    void resetObservations()
    {
        for (auto name : {"clicks", "doubles", "keys"})
            window->setProperty(name, 0);
        window->setProperty("selection", "");
        window->setProperty("selectedAtDouble", "");
        recorder.events.clear();
    }

    bool waitFor(const std::function<bool()>& predicate) { return QTest::qWaitFor(predicate, 2000); }
    void deliver()
    {
        executor.processCommands(scene);
        QCoreApplication::sendPostedEvents();
        QCoreApplication::processEvents();
    }

    const spix::ItemPath path {"gestureWindow/row"};
    QQmlEngine engine;
    EventRecorder recorder;
    std::unique_ptr<QQuickWindow> window;
    spix::QtScene scene;
    spix::CommandExecuter executor;
    Server server;
};

TEST_F(DoubleClickGesture, PositiveControlAndOrdinaryClickNegativeControl)
{
    const auto center = row()->mapToScene(QPointF(80, 40)).toPoint();
    QTest::mouseDClick(static_cast<QWindow*>(window.get()), Qt::LeftButton, Qt::NoModifier, center);
    ASSERT_TRUE(waitFor([&] { return count("doubles") == 1 && !row()->property("pressed").toBool(); }));
    EXPECT_EQ(window->property("selectedAtDouble").toString(), "row");

    resetObservations();
    server.mouseClick(path);
    server.mouseClick(path);
    deliver();
    EXPECT_EQ(count("clicks"), 2);
    EXPECT_EQ(count("doubles"), 0);
}

TEST_F(DoubleClickGesture, MatchesPositiveControlAndDeliversCompleteSequence)
{
    for (bool accept : {true, false}) {
        window->setProperty("acceptDouble", accept);
        resetObservations();
        QTest::mouseDClick(static_cast<QWindow*>(window.get()), Qt::LeftButton, Qt::NoModifier,
            row()->mapToScene(QPointF(80, 40)).toPoint());
        ASSERT_TRUE(waitFor([&] { return count("doubles") == 1 && !row()->property("pressed").toBool(); }));
        const auto ordinaryClicks = count("clicks");
        const auto controlEvents = recorder.events;
        ASSERT_EQ(controlEvents.size(), 5u);

        resetObservations();
        server.mouseDoubleClick(path);
        deliver();
        ASSERT_TRUE(waitFor([&] { return count("doubles") == 1 && !row()->property("pressed").toBool(); }));
        EXPECT_EQ(count("clicks"), ordinaryClicks);
        EXPECT_EQ(window->property("selectedAtDouble").toString(), "row");
        ASSERT_EQ(recorder.events.size(), 5u);
        const std::vector<QEvent::Type> expected {QEvent::MouseButtonPress, QEvent::MouseButtonRelease,
            QEvent::MouseButtonPress, QEvent::MouseButtonDblClick, QEvent::MouseButtonRelease};
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(recorder.events[i].type, expected[i]);
            EXPECT_EQ(recorder.events[i].position, row()->mapToScene(QPointF(80, 40)));
            EXPECT_EQ(recorder.events[i].button, Qt::LeftButton);
            EXPECT_EQ(recorder.events[i].buttons, controlEvents[i].buttons);
            EXPECT_EQ(recorder.events[i].modifiers, Qt::NoModifier);
        }
        server.mouseClick(path);
        deliver();
        EXPECT_EQ(count("clicks"), ordinaryClicks + 1);
        EXPECT_EQ(count("doubles"), 1);
        EXPECT_FALSE(row()->property("pressed").toBool());
    }
}

TEST_F(DoubleClickGesture, PositionsReachNestedTransformedTarget)
{
    auto* group = window->findChild<QQuickItem*>("group");
    group->setRotation(17);
    group->setScale(1.15);
    const std::vector<QPointF> expected {{80, 40}, {40, 20}, {50, 25}};
    for (size_t i = 0; i < expected.size(); ++i) {
        resetObservations();
        if (i == 0)
            server.mouseDoubleClick(path);
        else if (i == 1)
            server.mouseDoubleClick(path, {0.25, 0.25});
        else
            server.mouseDoubleClick(path, {0.25, 0.25}, {10, 5});
        deliver();
        ASSERT_TRUE(waitFor([&] { return count("doubles") == 1; }));
        EXPECT_NEAR(window->property("lastX").toDouble(), expected[i].x(), 0.01);
        EXPECT_NEAR(window->property("lastY").toDouble(), expected[i].y(), 0.01);
        EXPECT_FALSE(row()->property("pressed").toBool());
    }
}

TEST_F(DoubleClickGesture, GlobalPositionTracksWindowAndFractionalItemCoordinates)
{
    auto* group = window->findChild<QQuickItem*>("group");
    group->setRotation(17);
    group->setScale(1.15);
    const QPointF localPoint(40.25, 20.5);
    for (const auto& position : {QPoint(120, 160), QPoint(260, 210)}) {
        window->setPosition(position);
        QCoreApplication::processEvents();
        resetObservations();
        const auto expectedWindowPosition = row()->mapToScene(localPoint);
        const auto expectedGlobalPosition = row()->mapToGlobal(localPoint);

        server.mouseDoubleClick(path, {0.0, 0.0}, {localPoint.x(), localPoint.y()});
        deliver();

        ASSERT_EQ(count("doubles"), 1);
        ASSERT_EQ(recorder.events.size(), 5u);
        for (const auto& event : recorder.events) {
            EXPECT_NEAR(event.position.x(), expectedWindowPosition.x(), 0.01);
            EXPECT_NEAR(event.position.y(), expectedWindowPosition.y(), 0.01);
            EXPECT_NEAR(event.globalPosition.x(), expectedGlobalPosition.x(), 0.01);
            EXPECT_NEAR(event.globalPosition.y(), expectedGlobalPosition.y(), 0.01);
        }
    }
}

TEST_F(DoubleClickGesture, PointerHandlerReleasesAfterDoubleClick)
{
    auto* handler = window->findChild<QObject*>("tapHandler");
    ASSERT_NE(handler, nullptr);
    for (int gesture = 1; gesture <= 3; ++gesture) {
        server.mouseDoubleClick(spix::ItemPath("gestureWindow/tapTarget"));
        deliver();
        EXPECT_FALSE(handler->property("pressed").toBool());
        EXPECT_EQ(count("tapDoubles"), gesture);
    }
}

TEST_F(DoubleClickGesture, ForwardsEverySupportedButtonAndModifierCombination)
{
    for (auto button : {spix::MouseButtons::Left, spix::MouseButtons::Right, spix::MouseButtons::Middle}) {
        for (unsigned mod = 0; mod < 16; ++mod) {
            SCOPED_TRACE(testing::Message() << "button=" << button << " modifiers=" << mod);
            resetObservations();
            server.mouseDoubleClick(path, button, mod);
            deliver();
            ASSERT_TRUE(waitFor([&] { return count("doubles") == 1; }));
            auto qtButton = button == spix::MouseButtons::Left ? Qt::LeftButton
                : button == spix::MouseButtons::Right          ? Qt::RightButton
                                                               : Qt::MiddleButton;
            Qt::KeyboardModifiers qtMod = Qt::NoModifier;
            if (mod & spix::KeyModifiers::Shift)
                qtMod |= Qt::ShiftModifier;
            if (mod & spix::KeyModifiers::Control)
                qtMod |= Qt::ControlModifier;
            if (mod & spix::KeyModifiers::Alt)
                qtMod |= Qt::AltModifier;
            if (mod & spix::KeyModifiers::Meta)
                qtMod |= Qt::MetaModifier;
            EXPECT_EQ(count("lastButton"), qtButton);
            EXPECT_EQ(count("lastModifiers"), static_cast<int>(qtMod));
            ASSERT_EQ(recorder.events.size(), 5u);
            for (const auto& event : recorder.events) {
                EXPECT_EQ(event.button, qtButton);
                EXPECT_EQ(event.modifiers, qtMod);
            }
            EXPECT_FALSE(row()->property("pressed").toBool());
        }
    }
}

TEST_F(DoubleClickGesture, FollowingDragAndKeyboardInputStillWork)
{
    server.mouseDoubleClick(path);
    deliver();
    ASSERT_EQ(count("doubles"), 1);
    auto* tile = row()->parentItem();
    const auto initialX = tile->x();
    server.mouseBeginDrag(path);
    deliver();
    ASSERT_TRUE(row()->property("pressed").toBool());
    spix::QtItem item(row());
    scene.events().mouseMove(&item, {110, 40});
    QCoreApplication::sendPostedEvents();
    scene.events().mouseMove(&item, {130, 40});
    QCoreApplication::sendPostedEvents();
    EXPECT_GT(tile->x(), initialX);
    server.mouseEndDrag(path);
    deliver();
    EXPECT_FALSE(row()->property("pressed").toBool());
    row()->forceActiveFocus();
    server.enterKey(path, spix::KeyCodes::Return, spix::KeyModifiers::None);
    deliver();
    EXPECT_EQ(count("keys"), 1);
}

TEST_F(DoubleClickGesture, HeldButtonRejectsGestureWithoutDisturbingDrag)
{
    server.mouseBeginDrag(path);
    deliver();
    ASSERT_TRUE(row()->property("pressed").toBool());
    recorder.events.clear();
    server.mouseDoubleClick(path);
    deliver();
    EXPECT_TRUE(recorder.events.empty());
    EXPECT_EQ(count("doubles"), 0);
    EXPECT_TRUE(row()->property("pressed").toBool());
    server.mouseEndDrag(path);
    deliver();
    EXPECT_FALSE(row()->property("pressed").toBool());
    server.mouseDoubleClick(path);
    deliver();
    EXPECT_EQ(count("doubles"), 1);
}

TEST_F(DoubleClickGesture, DestroyingTargetDuringHandlerDoesNotLeaveMousePressed)
{
    QPointer<QQuickItem> oldRow(row());
    window->setProperty("removeOnDouble", true);
    server.mouseDoubleClick(path);
    deliver();
    ASSERT_TRUE(waitFor([&] { return oldRow.isNull(); }));
    EXPECT_EQ(count("doubles"), 1);
    window->setProperty("removeOnDouble", false);
    window->findChild<QObject*>("rowLoader")->setProperty("active", true);
    ASSERT_TRUE(waitFor([&] { return row() != nullptr; }));
    server.mouseClick(path);
    deliver();
    EXPECT_EQ(count("clicks"), 2);
    EXPECT_FALSE(row()->property("pressed").toBool());
    server.mouseDoubleClick(path);
    deliver();
    EXPECT_EQ(count("doubles"), 2);
}

TEST_F(DoubleClickGesture, MissingAndDetachedTargetsDoNotCorruptFollowingInput)
{
    server.mouseDoubleClick(spix::ItemPath("gestureWindow/missing"));
    deliver();
    EXPECT_TRUE(recorder.events.empty());
    QQuickItem detached;
    spix::QtItem item(&detached);
    EXPECT_FALSE(scene.events().mouseDoubleClick(&item, {0, 0}, spix::MouseButtons::Left, spix::KeyModifiers::None));
    server.mouseDoubleClick(path);
    deliver();
    EXPECT_EQ(count("doubles"), 1);
    EXPECT_FALSE(row()->property("pressed").toBool());
}

} // namespace

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
