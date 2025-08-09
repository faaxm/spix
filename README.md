[![Build and Test](https://github.com/faaxm/spix/actions/workflows/build.yml/badge.svg)](https://github.com/faaxm/spix/actions/workflows/build.yml)

<p align="center">
    <img alt="Spix Logo" src="https://www.f-ax.de/ext_files/spix_logo.png" height="110">
</p>

# Spix
Spix is a minimally invasive UI testing library that enables your
Qt/QML app's UI to be controlled either via c++ code, or through an http RPC
interface.

UI elements are referenced through names and paths which are robust against
design changes. To click on a button you write
```cpp
mouseClick("mainWindow/ok_button");
```

To provide an RPC test interface to your app,
only add these three lines to your `main(...)` function:
```cpp
spix::AnyRpcServer server;
auto bot = new spix::QtQmlBot();
bot->runTestServer(server);
```

And a test script in python could look like this:
```python
import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:9000')
s.mouseClick("mainWindow/Button_1")
s.wait(200)
s.mouseClick("mainWindow/Button_2")
resultText = s.getStringProperty("mainWindow/results", "text")
s.quit()
```

You can also use [PyAutoGUI](https://pyautogui.readthedocs.io) in combination with
Spix. Have a look at the [example script](examples/qtquick/RemoteCtrl/script/autogui.py).

## What are the applications of Spix?
The main use for Spix is to automatically test the UI of your Qt/QML application
and make sure that it behaves as you expect. However, you can also use Spix as
an easy way to remote control existing Qt/QML applications or to automatically
generate and update screenshots for your documentation.

# Requirements
* Qt (both 5 and 6 supported)
* [AnyRPC](https://github.com/sgieseking/anyrpc)

# Current Features
* Send mouse events (click, move, drag/drop)
* Drop mime data from external apps
* Enter text
* Check existence and visibility of items
* Get property values of items (text, position, color, ...)
* Invoke a method on an object
* Take and save a screenshot
* Quit the app
* Remote control, also of embedded devices / iOS

# Setting Up Spix

## Installing AnyRPC

Spix requires [AnyRPC](https://github.com/sgieseking/anyrpc) be installed on the local machine before building. For *nix machines, AnyRPC can be built/installed using CMake:
```sh
# in a temporary directory
git clone https://github.com/sgieseking/anyrpc.git
cd anyrpc
mkdir build
cd build
cmake -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DBUILD_PROTOCOL_MESSAGEPACK=OFF ..
cmake --build .
sudo cmake --install .
```

For non-*nix machines, checkout the [CI install script](ci/install-deps.sh).

## Installing Spix

Spix uses cmake and can be build with the standard cmake commands once cloned:
```sh
git clone https://github.com/faaxm/spix
cd spix
mkdir build && cd build
cmake -DSPIX_QT_MAJOR=6 ..
cmake --build .
sudo cmake --install .
```
> Change SPIX_QT_MAJOR to 5 to build against Qt5 instead of Qt6.

If you installed the dependencies (like AnyRPC) in a non-standard directory you can point cmake to it by setting `CMAKE_PREFIX_PATH`, so
instead of `cmake ..` you run:
```sh
cmake -DCMAKE_PREFIX_PATH=/path/to/libs ..
```

## Including Spix in your Qt project

If using qmake, add the following to your Qt `.pro` file:
```
QT += quick
INCLUDEPATH += /usr/local/include
LIBS += -lSpix -lanyrpc
```
If using CMake, add the following to your `CMakeLists.txt`:
```
find_package(Spix REQUIRED)
```

Update your `main(...)` to start the Spix RPC server:
```C++
#include <Spix/AnyRpcServer.h>
#include <Spix/QtQmlBot.h>

int main(...) {
    ...
    spix::AnyRpcServer server;
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(server);
    ...
}
```
Finally, if you're using a `QQuickView` as your root window, you'll need to give it an object name in your `main` (otherwise the root window object name will be defined in your QML):
```C++
int main(...) {
    QQuickView view;
    view.setObjectName("root")
    ...
}
```

# Using Spix

The easiest method of interacting with Spix is using the [XMLRPC client built into python](https://docs.python.org/3/library/xmlrpc.client.html#module-xmlrpc.client):
```python
import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:9000') # default port is 9000
s.method(<path>, <options>)
# for example:
s.mouseClick("root/Button_2")
resultText = s.getStringProperty("root/results", "text")
```

You can also use the XMLRPC client to list the available methods. The complete list of methods are also available in the [source](lib/src/AnyRpcServer.cpp).
```python
print(s.system.listMethods())
# ['command', 'enterKey', 'existsAndVisible', 'getBoundingBox', 'getErrors', 'getStringProperty', 'inputText', 'invokeMethod', 'mouseBeginDrag', 'mouseClick', 'mouseDropUrls', 'mouseEndDrag', 'quit', 'setStringProperty', 'system.listMethods', 'system.methodHelp', 'takeScreenshot', 'wait']
print(s.system.methodHelp('mouseClick'))
# Click on the object at the given path
```

Spix uses a slash-separated path format to select Qt objects. Selectors match against `objectName` or `id` if no object name is defined.
```
<root>/<child0>(/<childN>...)
```
Spix matches children recursivley, allowing as much flexibility as needed:
```
# matches any `button` that is a descendant of `root` (even subchildren)
'root/button'
# matches any `button` that is a descendant of `numberpad` which is in turn a descendant of `root`.
'root/numberpad/button'
# and so on
```

More specifically, Spix's matching processes works as follows:
* `<root>` matches a top-level [`QQuickWindow`](https://doc-snapshots.qt.io/qt6-dev/qquickwindow.html) whose `objectName` (or `id` if `objectName` is empty) matches the specified string. Top-level windows are enumerated by [`QGuiApplication::topLevelWindows`](https://doc.qt.io/qt-6/qguiapplication.html#topLevelWindows).
* `<child>` matches the first child object whose `objectName` (or `id` if `objectName` is empty) matches the specified string using a recursive search of all children and subchildren of the root. This process repeats for every subsequent child path entry.

### Invoking QML methods

Spix can directly invoke both internal and custom methods in QML objects: this can be a handy way to automate interactions that Spix doesn't support normally. For example, we can control the cursor in a `TextArea` by calling [`TextArea.select`](https://doc-snapshots.qt.io/qt6-6.2/qml-qtquick-textedit.html#select-method):
```qml
TextArea {
    id: textArea
}
```
```python
# select characters 100-200
s.invokeMethod("root/textArea", "select", [100, 200])
```

In addition, you can use custom functions in the QML to implement more complicated interactions, and have Spix interact with the function:
```qml
TextArea {
    id: textArea
    function customFunction(arg1, arg2) {
        // insert QML interactions here
        return {'key1': true, 'key2': false}
    }
}
```
```python
# invoke the custom function
result = s.invokeMethod("root/textArea", "customFunction", ['a string', 34])
# prints {'key1': True, 'key2': False}
print(result)
```

Spix supports the following types as arguments/return values:
| Python Type       | XMLRPC Type          | QML Type(s)     | JavaScript Type(s)| Notes                                            |
|-------------------|----------------------|-----------------|-------------------|--------------------------------------------------|
| int               | \<int\>              | int             | number            | Values over/under int max are upcasted to double |
| bool              | \<boolean\>          | bool            | boolean           |                                                  |
| str               | \<string\>           | string          | string            |                                                  |
| float             | \<double\>           | double, real    | number            | Defaults to double                               |
| datetime.datetime | \<dateTime.iso8601\> | date            | Date              | No timezone support (always uses local timezone) |
| dict              | \<struct\>           | var             | object            | String keys only                                 |
| list              | \<array\>            | var             | Array             |                                                  |
| None              | no type              | null, undefined | object, undefined | Defaults to null                                 |                              |

In general Spix will attempt to coerce the arguments and return value to the correct types to match the method being invoked. Valid conversion are listed under the [`QVariant` docs](https://doc.qt.io/qt-5/qvariant.html#canConvert). If Spix cannot find a valid conversion it will generate an error.
```qml
Item {
    id: item
    function test(arg1: bool) {
        ...
    }
}
```
```python
# ok
s.invokeMethod("root/item", "test", [False])

# argument will implicitly be converted to a boolean (True) to match the declaration type
s.invokeMethod("root/item", "test", [34])

# no conversion from object to boolean, so an error is thrown
s.invokeMethod("root/item", "test", [{}])
```

###  Using generic/custom command
You can register your own commands in your C++ Application.
It could be useful for Example to reset your hole Application.

Register the Commands in your C++ Code:
```C++
    ...
    spix::AnyRpcServer server;
    server.setGenericCommandHandler([](std::string command, std::string payload) {
        // do whatever needs to be done
    });
    ...
```
Now you have all capabilities that the Application has.
The Payload handling must be done by your own.

You can call this in Python like this:
```python
s.command('reset', 'now')
```

## Two modes of operation
In general, Spix can be used in two ways, which are different in how events are generated and sent
to your application:

### Generate Qt events directly
You can use Spix to directly create Qt events, either from C++ as a unit test, or from
an external script via the network through RPC. Since the Qt events are generated directly inside the
app, and do not come from the system, the mouse cursor will not actually move and interaction
with other applications is limited. On the plus side, this mechanism is independent from
the system your app is running on and can easily be used to control software on an embedded
device via the network (RPC).

### Generate system events externally
In this case, Spix is not generating the events itself. Instead, you use a script to query
Spix for the screen coordinates of qt objects and then generate events on the system level
through other tools. One option is to use python together with PyAutoGUI for this, as is
done in the [RemoteCtrl](examples/qtquick/RemoteCtrl) example.
