# RPC API Reference

Spix provides an XMLRPC interface for remote control of Qt applications.

## Connecting to Spix

Use Python's built-in XMLRPC client:

```python
import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:9000')
```

The default port is `9000`. You can change it when constructing `AnyRpcServer`:

```cpp
spix::AnyRpcServer server(8080);  // Use port 8080
```

## Discovering Methods

```python
# List all available methods
print(s.system.listMethods())

# Get help for a specific method
print(s.system.methodHelp('mouseClick'))
```

## Method Reference

### Mouse Events

| Method | Signature | Description |
|--------|-----------|-------------|
| `mouseClick` | `mouseClick(path)` | Click at item center |
| `mouseClickWithButton` | `mouseClickWithButton(path, button, modifier)` | Click with specific button and modifiers |
| `mouseClickWithOffset` | `mouseClickWithOffset(path, offsetX, offsetY)` | Click with pixel offset from top-left |
| `mouseClickWithProportion` | `mouseClickWithProportion(path, propX, propY)` | Click at proportional position (0.0-1.0) |
| `mouseBeginDrag` | `mouseBeginDrag(path)` | Begin drag operation |
| `mouseEndDrag` | `mouseEndDrag(path)` | End drag operation |
| `mouseDropUrls` | `mouseDropUrls(path, [url1, url2, ...])` | Drop URLs (file paths) onto item |

**Mouse buttons:** `1` = Left, `2` = Right, `4` = Middle

**Key modifiers:** `0` = None, `1` = Shift, `2` = Control, `4` = Alt, `8` = Meta

```python
# Basic click
s.mouseClick("mainWindow/button")

# Right-click with Ctrl held
s.mouseClickWithButton("mainWindow/item", 2, 2)

# Click 10 pixels right and 5 pixels down from top-left
s.mouseClickWithOffset("mainWindow/item", 10, 5)

# Click at 75% width, 50% height
s.mouseClickWithProportion("mainWindow/item", 0.75, 0.5)

# Drag and drop
s.mouseBeginDrag("mainWindow/source")
s.mouseEndDrag("mainWindow/target")

# Drop files
s.mouseDropUrls("mainWindow/dropArea", ["file:///path/to/file.txt"])
```

### Text Input

| Method | Signature | Description |
|--------|-----------|-------------|
| `inputText` | `inputText(path, text)` | Type text into the focused widget |
| `enterKey` | `enterKey(path, keyCode, modifier)` | Press and release a key |

```python
# Type text
s.inputText("mainWindow/textField", "Hello World")

# Press Enter (Qt::Key_Return = 16777220)
s.enterKey("mainWindow/textField", 16777220, 0)

# Press Ctrl+A (Qt::Key_A = 65)
s.enterKey("mainWindow/textField", 65, 2)
```

### Properties

| Method | Signature | Description |
|--------|-----------|-------------|
| `getStringProperty` | `getStringProperty(path, property) -> string` | Get property value as string |
| `setStringProperty` | `setStringProperty(path, property, value)` | Set property value |
| `getBoundingBox` | `getBoundingBox(path) -> [x, y, width, height]` | Get item bounds in screen coordinates |
| `existsAndVisible` | `existsAndVisible(path) -> bool` | Check if item exists and is visible |

```python
# Read text property
text = s.getStringProperty("mainWindow/label", "text")

# Set property
s.setStringProperty("mainWindow/label", "text", "New Text")

# Get position for external automation
bbox = s.getBoundingBox("mainWindow/button")
x, y, width, height = bbox

# Check existence
if s.existsAndVisible("mainWindow/dialog"):
    s.mouseClick("mainWindow/dialog/okButton")
```

### Method Invocation (QtQuick)

| Method | Signature | Description |
|--------|-----------|-------------|
| `invokeMethod` | `invokeMethod(path, method, args) -> result` | Call a QML method |

```python
# Call built-in method
s.invokeMethod("mainWindow/textArea", "select", [100, 200])

# Call custom QML function
result = s.invokeMethod("mainWindow/item", "customFunction", ["arg1", 42])
```

See [QtQuick Guide](qtquick-guide.md#invoking-qml-methods) for type conversion details.

### Timing

| Method | Signature | Description |
|--------|-----------|-------------|
| `wait` | `wait(milliseconds)` | Wait for specified time |
| `waitForItem` | `waitForItem(path, timeout) -> bool` | Wait for item to appear |

```python
# Fixed wait
s.wait(500)

# Wait up to 5 seconds for item
if s.waitForItem("mainWindow/loadedContent", 5000):
    print("Content loaded")
else:
    print("Timeout")
```

### Screenshots

| Method | Signature | Description |
|--------|-----------|-------------|
| `takeScreenshot` | `takeScreenshot(path, filePath)` | Save screenshot to file |
| `takeScreenshotAsBase64` | `takeScreenshotAsBase64(path) -> string` | Get screenshot as base64 |

```python
# Save to file
s.takeScreenshot("mainWindow", "/tmp/screenshot.png")

# Get as base64
import base64
b64 = s.takeScreenshotAsBase64("mainWindow")
image_data = base64.b64decode(b64)
```

### Error Handling

| Method | Signature | Description |
|--------|-----------|-------------|
| `getErrors` | `getErrors() -> [string, ...]` | Get accumulated errors |

```python
errors = s.getErrors()
if errors:
    print("Errors occurred:", errors)
```

### Application Control

| Method | Signature | Description |
|--------|-----------|-------------|
| `quit` | `quit()` | Close the application |
| `command` | `command(name, payload)` | Execute custom command |

```python
# Quit app
s.quit()

# Custom command (requires handler registration)
s.command("reset", "full")
```

## Custom Commands

Register handlers in your C++ code:

```cpp
spix::AnyRpcServer server;
server.setGenericCommandHandler([](std::string command, std::string payload) {
    if (command == "reset") {
        // Handle reset command
    }
});
```

```python
s.command("reset", "")
```

## Two Modes of Operation

### 1. Direct Qt Events (Default)

Spix generates Qt events directly inside the application. The mouse cursor does not move visually, but events are delivered correctly. This works on any platform and over the network.

### 2. External System Events

Query Spix for screen coordinates, then use external tools like PyAutoGUI to generate real mouse/keyboard events:

```python
import pyautogui
import xmlrpc.client

s = xmlrpc.client.ServerProxy('http://localhost:9000')

# Get screen position
bbox = s.getBoundingBox("mainWindow/button")
center_x = bbox[0] + bbox[2] / 2
center_y = bbox[1] + bbox[3] / 2

# Generate real mouse event
pyautogui.click(center_x, center_y)
```

See [RemoteCtrl example](../examples/qtquick/RemoteCtrl) for a complete implementation.
