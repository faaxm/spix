# Getting Started with Spix

This guide covers installing Spix and integrating it into your Qt project.

## Requirements

- [AnyRPC](https://github.com/sgieseking/anyrpc) (required for RPC functionality)
- Qt 5.15+ or Qt 6.x (for scene libraries)
- CMake 3.18+
- C++17 compiler

## Installing AnyRPC

Spix requires AnyRPC to be installed before building.

### Linux / macOS

```sh
git clone https://github.com/sgieseking/anyrpc.git
cd anyrpc
mkdir build && cd build
cmake -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DBUILD_PROTOCOL_MESSAGEPACK=OFF ..
cmake --build .
sudo cmake --install .
```

### Windows

See the [CI install script](../ci/install-deps.sh) for Windows-specific instructions.

## Building Spix

```sh
git clone https://github.com/faaxm/spix
cd spix
mkdir build && cd build
cmake -DSPIX_QT_MAJOR=6 ..
cmake --build .
sudo cmake --install .
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `SPIX_QT_MAJOR` | `6` | Qt version to build against (5 or 6) |
| `SPIX_BUILD_QTQUICK` | `ON` | Build QtQuick scene support |
| `SPIX_BUILD_QTWIDGETS` | `OFF` | Build QtWidgets scene support (Qt6 only) |
| `SPIX_BUILD_EXAMPLES` | `ON` | Build example applications |
| `SPIX_BUILD_TESTS` | `OFF` | Build unit tests |

### Build Configurations

**Standard build (QtQuick only):**
```sh
cmake -DSPIX_QT_MAJOR=6 ..
```

**With QtWidgets support (Qt6 only):**
```sh
cmake -DSPIX_QT_MAJOR=6 -DSPIX_BUILD_QTWIDGETS=ON ..
```

**Core-only build (no Qt dependencies):**
```sh
cmake -DSPIX_BUILD_QTQUICK=OFF ..
```

### Custom Dependency Paths

If you installed dependencies in a non-standard location:
```sh
cmake -DCMAKE_PREFIX_PATH=/path/to/libs ..
```

## Including Spix in Your Project

### CMake (Recommended)

**For Qt/QML applications:**
```cmake
find_package(Spix REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtQuick)
```

**For QWidget applications (Qt6 only):**
```cmake
find_package(SpixQtWidgets REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtWidgets)
```

**Component-based usage:**
```cmake
# Specific components
find_package(Spix COMPONENTS Core QtQuick REQUIRED)
target_link_libraries(your_app PRIVATE Spix::QtQuick)

# Core-only (no Qt)
find_package(Spix COMPONENTS Core REQUIRED)
target_link_libraries(your_app PRIVATE Spix::Core)
```

### qmake

```
QT += quick
INCLUDEPATH += /usr/local/include
LIBS += -lSpixQtQuick -lSpixCore -lanyrpc
```

## Next Steps

- [QtQuick Guide](qtquick-guide.md) - Using Spix with Qt/QML applications
- [QtWidgets Guide](qtwidgets-guide.md) - Using Spix with QWidget applications
- [RPC API Reference](rpc-api.md) - Remote control via XMLRPC
- [Item Path System](item-path.md) - How to address UI elements
