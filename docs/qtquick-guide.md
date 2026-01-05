# QtQuick Guide

This guide covers using Spix with Qt/QML (QtQuick) applications.

## Overview

The `Spix::QtQuick` library provides UI testing capabilities for Qt/QML applications. It uses `QtQmlBot` to process commands and interact with QML items through the `QtScene` implementation.

## Requirements

- Qt 5.15+ or Qt 6.x
- `SPIX_BUILD_QTQUICK=ON` (default)

## Setup

### CMake Integration

```cmake
find_package(Spix REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtQuick)
```

### Basic Integration

Add Spix to your `main()` function:

```cpp
#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Set up Spix
    spix::AnyRpcServer server;
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(server);

    engine.load(QUrl("qrc:/main.qml"));
    return app.exec();
}
```

### Window Object Names

If using `QQuickView` as your root window, set its object name explicitly:

```cpp
QQuickView view;
view.setObjectName("root");
view.setSource(QUrl("qrc:/main.qml"));
view.show();
```

Otherwise, the root window name is defined in your QML file.

## Addressing QML Items

Spix uses slash-separated paths to identify QML items. Items are matched by their `objectName` property (or `id` if no objectName is set).

```qml
// main.qml
ApplicationWindow {
    objectName: "mainWindow"

    Button {
        objectName: "submitButton"
        text: "Submit"
    }
}
```

```python
# Test script
s.mouseClick("mainWindow/submitButton")
```

The path does not need to include every element in the hierarchy. Spix performs a depth-first search to find matching items:

```python
# These can match items at any depth
s.mouseClick("mainWindow/submitButton")  # Works even if button is nested
s.mouseClick("mainWindow/form/submitButton")  # More specific path
```

For advanced path selectors (type, property, value), see [Item Path System](item-path.md).

## Invoking QML Methods

Spix can invoke methods on QML objects directly:

```qml
TextArea {
    id: textArea

    function customFunction(arg1, arg2) {
        // Custom logic
        return {'result': true}
    }
}
```

```python
# Invoke built-in method
s.invokeMethod("mainWindow/textArea", "select", [100, 200])

# Invoke custom function
result = s.invokeMethod("mainWindow/textArea", "customFunction", ['hello', 42])
```

### Type Conversions

| Python Type | QML Type(s) | Notes |
|-------------|-------------|-------|
| `int` | int | Large values become double |
| `bool` | bool | |
| `str` | string | |
| `float` | double, real | |
| `datetime.datetime` | date | Local timezone only |
| `dict` | var (object) | String keys only |
| `list` | var (Array) | |
| `None` | null, undefined | |

Spix attempts to coerce arguments to match the method signature. See [QVariant docs](https://doc.qt.io/qt-6/qvariant.html#canConvert) for valid conversions.

## C++ Unit Testing with GTest

You can use Spix directly from C++ for unit testing:

```cpp
#include <Spix/QtQmlBot.h>
#include <Spix/TestServer.h>
#include <gtest/gtest.h>

class SpixGTest : public spix::TestServer {
protected:
    void executeTest() override {
        srv = this;
        ::testing::InitGoogleTest(&m_argc, m_argv);
        m_result = RUN_ALL_TESTS();
    }
public:
    static SpixGTest* srv;
    int m_result = 0;
    int m_argc;
    char** m_argv;
};

TEST(MyTest, ClickButton) {
    SpixGTest::srv->mouseClick(spix::ItemPath("mainWindow/button"));
    SpixGTest::srv->wait(std::chrono::milliseconds(100));

    auto text = SpixGTest::srv->getStringProperty("mainWindow/label", "text");
    EXPECT_EQ(text, "Clicked!");
}
```

See [examples/qtquick/GTest](../examples/qtquick/GTest) for a complete example.

## Examples

The following examples demonstrate QtQuick usage:

- [Basic](../examples/qtquick/Basic) - Simple button click example
- [GTest](../examples/qtquick/GTest) - GTest integration
- [RemoteCtrl](../examples/qtquick/RemoteCtrl) - Remote control with PyAutoGUI
- [RepeaterLoader](../examples/qtquick/RepeaterLoader) - Working with Repeater/Loader items

## How It Works

`QtQmlBot` creates a timer that periodically processes commands from the `CommandExecuter`. When a command needs to interact with the UI:

1. `QtScene` locates items using `QGuiApplication::topLevelWindows()`
2. `QtItem` wraps `QQuickItem` instances for property access and method invocation
3. `QtEvents` generates Qt events via `QGuiApplication::postEvent()`

For architectural details, see [Architecture](architecture.md).
