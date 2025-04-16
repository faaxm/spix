/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/AnyRpcServer.h>
#include <Spix/Data/Variant.h>
#include <Utils/AnyRpcFunction.h>
#include <atomic>

namespace spix {

struct AnyRpcServerPimpl {
    std::unique_ptr<anyrpc::Server> server;
    std::mutex serverAccessMutex;
    std::atomic<bool> keepRunning {true};
};

AnyRpcServer::AnyRpcServer(int anyrpcPort)
: m_pimpl(new AnyRpcServerPimpl())
{
    m_pimpl->server = std::make_unique<anyrpc::XmlHttpServer>();

    anyrpc::MethodManager* methodManager = m_pimpl->server->GetMethodManager();

    utils::AddFunctionToAnyRpc<void(int)>(methodManager, "wait",
        "Wait given number of milliseconds | wait(int millisecondsToWait)",
        [this](int ms) { wait(std::chrono::milliseconds(ms)); });

    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseClick",
        "Click on the object at the given path | mouseClick(string path)",
        [this](std::string path) { mouseClick(std::move(path)); });

    utils::AddFunctionToAnyRpc<void(std::string, int, int)>(methodManager, "mouseClickWithButton",
        "Click on the object at the given path with the given mouse button and Control Keys | "
        "mouseClickWithButton(string path, int mouseButton)",
        [this](std::string path, int mouseButton, int keyModifier = KeyModifiers::None) {
            mouseClick(std::move(path), mouseButton, keyModifier);
        });

    utils::AddFunctionToAnyRpc<void(std::string, double, double)>(methodManager, "mouseClickWithOffset",
        "Click on the object at the given path with the given offset"
        "(absolute pixel) | mouseClickWithOffset(string path, float "
        "offsetX, float offsetY)",
        [this](std::string path, double offsetX, double offsetY) {
            auto proportion = Point(0, 0);
            auto offset = Point(offsetX, offsetY);
            mouseClick(std::move(path), proportion, offset);
        });

    utils::AddFunctionToAnyRpc<void(std::string, double, double)>(methodManager, "mouseClickWithProportion",
        "Click on the object at the given path with the given proportion (In percent)"
        "| mouseClickWithProportion(string path, float proportionX, float proportionY)",
        [this](std::string path, double proportionX, double proportionY) {
            auto proportion = Point(proportionX, proportionY);
            mouseClick(std::move(path), proportion);
        });

    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseBeginDrag",
        "Begin a drag with the mouse | mouseBeginDrag(string path)",
        [this](std::string path) { mouseBeginDrag(std::move(path)); });

    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseEndDrag",
        "End a drag with the mouse | mouseEndDrag(string path)",
        [this](std::string path) { mouseEndDrag(std::move(path)); });

    utils::AddFunctionToAnyRpc<void(std::string, std::vector<std::string>)>(methodManager, "mouseDropUrls",
        "Drop Urls with mouse | mouseDropUrls(string path, [string url1, ...])",
        [this](std::string path, std::vector<std::string> urls) { mouseDropUrls(std::move(path), urls); });

    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "inputText",
        "Enter text. Events are sent to the item's window at path. | inputText(string path, string text)",
        [this](std::string path, std::string text) { inputText(std::move(path), std::move(text)); });

    utils::AddFunctionToAnyRpc<void(std::string, int, unsigned)>(methodManager, "enterKey",
        "Press and release a key | enterKey(string path, int keyCode, unsigned int keyModifier)",
        [this](std::string path, int keyCode, unsigned modifiers) { enterKey(std::move(path), keyCode, modifiers); });

    utils::AddFunctionToAnyRpc<std::string(std::string, std::string)>(methodManager, "getStringProperty",
        "Return a property as string | getStringProperty(string path, string property) : string property_value",
        [this](std::string path, std::string property) {
            return getStringProperty(std::move(path), std::move(property));
        });

    utils::AddFunctionToAnyRpc<void(std::string, std::string, std::string)>(methodManager, "setStringProperty",
        "Set the string value of the given property | setStringProperty(string path, string property, string "
        "new_value)",
        [this](std::string path, std::string property, std::string value) {
            setStringProperty(std::move(path), std::move(property), std::move(value));
        });

    utils::AddFunctionToAnyRpc<Variant(std::string, std::string, std::vector<Variant>)>(methodManager, "invokeMethod",
        "Invoke a method on a QML object | invokeMethod(string path, string method, any[] args)",
        [this](std::string path, std::string method, std::vector<Variant> args) {
            return invokeMethod(std::move(path), std::move(method), std::move(args));
        });

    utils::AddFunctionToAnyRpc<std::vector<double>(std::string)>(methodManager, "getBoundingBox",
        "Return the bounding box of an item in screen coordinates | getBoundingBox(string path) : (doubles) "
        "[topLeft.x, topLeft.y , width, height]",
        [this](std::string path) {
            auto bounds = getBoundingBox(std::move(path));
            return std::vector<double> {bounds.topLeft.x, bounds.topLeft.y, bounds.size.width, bounds.size.height};
        });

    utils::AddFunctionToAnyRpc<bool(std::string)>(methodManager, "existsAndVisible",
        "Returns true if the given object exists | existsAndVisible(string path) : bool exists_and_visible",
        [this](std::string path) { return existsAndVisible(std::move(path)); });

    utils::AddFunctionToAnyRpc<bool(std::string, int)>(methodManager, "waitForItem",
        "Returns true if the given object exists and the time is not expired | waitForItem(string path, int "
        "millisecondsToWait) : bool exists_and_visible",
        [this](std::string path, int ms) { return waitForItem(std::move(path), std::chrono::milliseconds(ms)); });

    utils::AddFunctionToAnyRpc<std::vector<std::string>()>(methodManager, "getErrors",
        "Returns internal errors that occurred during test execution | getErrors() : (strings) [error1, ...]",
        [this]() { return getErrors(); });

    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "takeScreenshot",
        "Take a screenshot of the object and save it as a file | takeScreenshot(string pathToTargetedItem, string "
        "filePath)",
        [this](std::string targetItem, std::string filePath) {
            return takeScreenshot(std::move(targetItem), std::move(filePath));
        });

    utils::AddFunctionToAnyRpc<std::string(std::string)>(methodManager, "takeScreenshotAsBase64",
        "Take a screenshot of the object and send as base64 string | takeScreenshotAsBase64(string pathToTargetedItem)",
        [this](std::string targetItem) { return takeScreenshotAsBase64(std::move(targetItem)); });

    utils::AddFunctionToAnyRpc<void()>(methodManager, "quit", "Close the app | quit()", [this] { quit(); });

    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "command",
        "Executes a generic/custom command | command(string command, string payload)",
        [this](std::string command, std::string payload) { genericCommand(command, payload); });

    m_pimpl->server->BindAndListen(anyrpcPort);
}

AnyRpcServer::~AnyRpcServer()
{
    m_pimpl->keepRunning.store(false);
    // make sure we exited executeTest at which point we get a lock
    std::lock_guard<std::mutex> lock(m_pimpl->serverAccessMutex);
}

void AnyRpcServer::executeTest()
{
    std::lock_guard<std::mutex> lock(m_pimpl->serverAccessMutex);
    while (m_pimpl->keepRunning.load()) {
        m_pimpl->server->Work(1000);
    }
}

} // namespace spix
