[![Build and Test](https://github.com/faaxm/spix/actions/workflows/build.yml/badge.svg)](https://github.com/faaxm/spix/actions/workflows/build.yml)

<p align="center">
    <img alt="Spix Logo" src="https://www.f-ax.de/ext_files/spix_logo.png" height="110">
</p>

# Spix

Spix is a minimally invasive UI testing library that enables your Qt app's UI to be controlled either via C++ code or through an HTTP RPC interface.

UI elements are referenced through names and paths which are robust against design changes:
```cpp
mouseClick("mainWindow/ok_button");
```

To provide an RPC test interface, add three lines to your `main()`:
```cpp
spix::AnyRpcServer server;
auto bot = new spix::QtQmlBot();
bot->runTestServer(server);
```

Test scripts can then control your app remotely:
```python
import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:9000')
s.mouseClick("mainWindow/Button_1")
s.wait(200)
resultText = s.getStringProperty("mainWindow/results", "text")
s.quit()
```

## Architecture

Spix uses a modular architecture:

- **Spix::Core** - Qt-independent functionality (RPC server, commands)
- **Spix::QtQuick** - Qt/QML scene implementation
- **Spix::QtWidgets** - QWidget scene implementation (Qt6 only)

This enables Core-only usage in headless environments and support for different Qt UI frameworks.

## Features

- Send mouse events (click, move, drag/drop)
- Drop mime data from external apps
- Enter text and key events
- Check existence and visibility of items
- Get/set property values
- Invoke methods on objects
- Take screenshots
- Remote control over network

## Requirements

- [AnyRPC](https://github.com/sgieseking/anyrpc)
- Qt 5.15+ or Qt 6.x (for scene libraries)
- CMake 3.18+

## Quick Start

```sh
# Install AnyRPC first (see docs/getting-started.md)

# Build Spix
git clone https://github.com/faaxm/spix
cd spix && mkdir build && cd build
cmake -DSPIX_QT_MAJOR=6 ..
cmake --build .
sudo cmake --install .
```

**Build Options:**
- `SPIX_QT_MAJOR`: Qt version (5 or 6, default: 6)
- `SPIX_BUILD_QTQUICK`: Build QtQuick support (default: ON)
- `SPIX_BUILD_QTWIDGETS`: Build QtWidgets support (default: OFF, Qt6 only)

## CMake Integration

**Qt/QML applications:**
```cmake
find_package(Spix REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtQuick)
```

**QWidget applications (Qt6 only):**
```cmake
find_package(SpixQtWidgets REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtWidgets)
```

## Item Paths

Spix uses slash-separated paths to identify UI elements:
```python
s.mouseClick("mainWindow/submitButton")
s.getStringProperty("mainWindow/form/resultLabel", "text")
```

Items are matched by `objectName` (or QML `id`). Paths don't need to include every ancestor - Spix searches recursively. See [Item Path System](docs/item-path.md) for advanced selectors.

## Documentation

- [Getting Started](docs/getting-started.md) - Installation and build instructions
- [QtQuick Guide](docs/qtquick-guide.md) - Using Spix with Qt/QML
- [QtWidgets Guide](docs/qtwidgets-guide.md) - Using Spix with QWidgets
- [RPC API Reference](docs/rpc-api.md) - Complete method reference
- [Item Path System](docs/item-path.md) - Path syntax and selectors
- [Architecture](docs/architecture.md) - Design overview

## Examples

- [QtQuick Examples](examples/qtquick/) - Basic, GTest, RemoteCtrl, RepeaterLoader
- [QtWidgets Examples](examples/qtwidgets/) - Basic, GTest, FormExample, DialogExample

## License

MIT License - see [LICENSE.txt](LICENSE.txt)
