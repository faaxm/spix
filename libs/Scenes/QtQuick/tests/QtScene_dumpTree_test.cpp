#include <gtest/gtest.h>

#include <QtScene.h>

#include <QGuiApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickItem>
#include <QQuickWindow>

class QtSceneDumpTreeTest : public ::testing::Test {
protected:
    QtSceneDumpTreeTest()
    : fakeArg {"hello"}
    , fakeArgPtr(fakeArg)
    , fakeArgList(&fakeArgPtr)
    , fakeArgCount(1)
    {
        qputenv("QT_QPA_PLATFORM", "offscreen");
        app = std::make_unique<QGuiApplication>(fakeArgCount, fakeArgList);
    }

    char fakeArg[50];
    char* fakeArgPtr;
    char** fakeArgList;
    int fakeArgCount;
    std::unique_ptr<QGuiApplication> app;
    spix::QtScene scene;
};

TEST_F(QtSceneDumpTreeTest, EmptyPathReturnsEmptyJson)
{
    auto result = scene.dumpTree(spix::ItemPath(""));
    EXPECT_EQ(result, "{}");
}

TEST_F(QtSceneDumpTreeTest, UnknownWindowReturnsEmptyJson)
{
    auto result = scene.dumpTree(spix::ItemPath("nonExistentWindow"));
    EXPECT_EQ(result, "{}");
}

TEST_F(QtSceneDumpTreeTest, WindowPathReturnsValidJson)
{
    QQuickWindow window;
    window.setObjectName("testWindow");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(QByteArray::fromStdString(result), &parseError);
    EXPECT_EQ(parseError.error, QJsonParseError::NoError) << result;
    EXPECT_TRUE(doc.isObject());
}

TEST_F(QtSceneDumpTreeTest, WindowRootNodeHasExpectedFields)
{
    QQuickWindow window;
    window.setObjectName("testWindow");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    EXPECT_TRUE(root.contains("type"));
    EXPECT_TRUE(root.contains("objectName"));
    EXPECT_TRUE(root.contains("className"));
    EXPECT_TRUE(root.contains("children"));
    EXPECT_TRUE(root.contains("properties"));
}

TEST_F(QtSceneDumpTreeTest, ChildItemAppearsInJson)
{
    QQuickWindow window;
    window.setObjectName("testWindow");

    auto* child = new QQuickItem(window.contentItem());
    child->setObjectName("myChild");
    child->setWidth(100);
    child->setHeight(50);

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    auto children = root["children"].toArray();
    ASSERT_FALSE(children.isEmpty());

    bool foundChild = false;
    for (const auto& childVal : children) {
        if (childVal.toObject()["objectName"].toString() == "myChild") {
            foundChild = true;
            auto childObj = childVal.toObject();
            EXPECT_TRUE(childObj.contains("bounds"));
            auto bounds = childObj["bounds"].toObject();
            EXPECT_EQ(bounds["width"].toDouble(), 100.0);
            EXPECT_EQ(bounds["height"].toDouble(), 50.0);
        }
    }
    EXPECT_TRUE(foundChild) << "Expected child named 'myChild' in JSON:\n" << result;
}

TEST_F(QtSceneDumpTreeTest, DeepChildAppearsInJson)
{
    QQuickWindow window;
    window.setObjectName("testWindow");

    auto* parent = new QQuickItem(window.contentItem());
    parent->setObjectName("parentItem");
    auto* child = new QQuickItem(parent);
    child->setObjectName("deepChild");

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    // Find parentItem in top-level children, then deepChild within it
    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    auto topChildren = root["children"].toArray();

    bool foundDeep = false;
    for (const auto& topVal : topChildren) {
        auto topObj = topVal.toObject();
        if (topObj["objectName"].toString() == "parentItem") {
            for (const auto& nestedVal : topObj["children"].toArray()) {
                if (nestedVal.toObject()["objectName"].toString() == "deepChild") {
                    foundDeep = true;
                }
            }
        }
    }
    EXPECT_TRUE(foundDeep) << "Expected nested child 'deepChild' in JSON:\n" << result;
}

TEST_F(QtSceneDumpTreeTest, VisibilityAndEnabledReflectedInJson)
{
    QQuickWindow window;
    window.setObjectName("testWindow");

    auto* item = new QQuickItem(window.contentItem());
    item->setObjectName("toggledItem");
    item->setVisible(true);
    item->setEnabled(false);

    auto result = scene.dumpTree(spix::ItemPath("testWindow"));

    auto root = QJsonDocument::fromJson(QByteArray::fromStdString(result)).object();
    bool foundItem = false;
    for (const auto& childVal : root["children"].toArray()) {
        auto childObj = childVal.toObject();
        if (childObj["objectName"].toString() == "toggledItem") {
            foundItem = true;
            EXPECT_TRUE(childObj["visible"].toBool());
            EXPECT_FALSE(childObj["enabled"].toBool());
        }
    }
    EXPECT_TRUE(foundItem);
}
