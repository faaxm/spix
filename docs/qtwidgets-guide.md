# QtWidgets Guide

This guide covers using Spix with traditional QWidget-based applications.

## Overview

The `Spix::QtWidgets` library provides UI testing capabilities for QWidget applications. It uses `QtWidgetsBot` to process commands and interact with widgets through the `QtWidgetsScene` implementation.

## Requirements

- **Qt 6.x only** (QtWidgets scene is not available for Qt 5)
- `SPIX_BUILD_QTWIDGETS=ON` (disabled by default)

## Building with QtWidgets Support

```sh
cmake -DSPIX_QT_MAJOR=6 -DSPIX_BUILD_QTWIDGETS=ON ..
cmake --build .
```

## Setup

### CMake Integration

```cmake
find_package(SpixQtWidgets REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtWidgets)
```

### Basic Integration

Add Spix to your `main()` function:

```cpp
#include <Spix/AnyRpcServer.h>
#include <Spix/QtWidgetsBot.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setObjectName("mainWindow");  // Important for path lookup
    window.show();

    // Set up Spix
    spix::AnyRpcServer server;
    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(server);

    return app.exec();
}
```

## Addressing Widgets

Spix uses slash-separated paths to identify widgets. Widgets are matched by their `objectName` property.

```cpp
// Setting up widgets with object names
auto button = new QPushButton("Submit", this);
button->setObjectName("submitButton");

auto label = new QLabel("Result", this);
label->setObjectName("resultLabel");
```

```python
# Test script
s.mouseClick("mainWindow/submitButton")
text = s.getStringProperty("mainWindow/resultLabel", "text")
```

The path does not need to include every widget in the hierarchy. Spix performs a depth-first search:

```python
# These can match widgets at any depth
s.mouseClick("mainWindow/submitButton")
s.mouseClick("mainWindow/formWidget/submitButton")
```

For advanced path selectors (type, property, value), see [Item Path System](item-path.md).

### Widget Type Names

When using type selectors, widget class names have the "Q" prefix stripped:

| Qt Class | Spix Type Name |
|----------|----------------|
| `QPushButton` | `PushButton` |
| `QLabel` | `Label` |
| `QLineEdit` | `LineEdit` |
| `QCheckBox` | `CheckBox` |
| `QComboBox` | `ComboBox` |

```python
# Match by type
s.mouseClick("mainWindow/#PushButton")  # First QPushButton descendant
```

## C++ Unit Testing with GTest

```cpp
#include <Spix/QtWidgetsBot.h>
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

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    SpixGTest tests;
    tests.m_argc = argc;
    tests.m_argv = argv;

    auto bot = new spix::QtWidgetsBot();
    bot->runTestServer(tests);

    app.exec();
    return tests.m_result;
}
```

See [examples/qtwidgets/GTest](../examples/qtwidgets/GTest) for a complete example.

## Examples

The following examples demonstrate QtWidgets usage:

- [Basic](../examples/qtwidgets/Basic) - Simple button click example
- [GTest](../examples/qtwidgets/GTest) - GTest integration
- [FormExample](../examples/qtwidgets/FormExample) - Forms with checkboxes, comboboxes
- [DialogExample](../examples/qtwidgets/DialogExample) - Modal dialogs and window switching

## Differences from QtQuick

| Aspect | QtQuick | QtWidgets |
|--------|---------|-----------|
| Qt Version | Qt 5.15+ or Qt 6.x | Qt 6.x only |
| Bot Class | `QtQmlBot` | `QtWidgetsBot` |
| Include | `<Spix/QtQmlBot.h>` | `<Spix/QtWidgetsBot.h>` |
| App Class | `QGuiApplication` | `QApplication` |
| Window Lookup | `QGuiApplication::topLevelWindows()` | `QApplication::topLevelWidgets()` |
| Type Prefix | Strip "QQuick" | Strip "Q" |

## How It Works

`QtWidgetsBot` creates a timer that periodically processes commands from the `CommandExecuter`. When a command needs to interact with the UI:

1. `QtWidgetsScene` locates widgets using `QApplication::topLevelWidgets()`
2. `QtWidgetsItem` wraps `QWidget` instances for property access and method invocation
3. `QtWidgetsEvents` generates Qt events via `QApplication::postEvent()` with proper targeting using `childAt()`

For architectural details, see [Architecture](architecture.md).
