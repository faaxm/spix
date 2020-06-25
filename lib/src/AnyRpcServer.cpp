/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/AnyRpcServer.h>
#include <Utils/AnyRpcUtils.h>
#include <anyrpc/anyrpc.h>
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

    utils::AddFunctionToAnyRpc<void(int)>(methodManager, "wait", "Wait given number of milliseconds",
        [this](int ms) { wait(std::chrono::milliseconds(ms)); });

    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseClick", "Click on the object at the given path",
        [this](std::string path) { mouseClick(std::move(path)); });
    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseBeginDrag", "Begin a drag with the mouse",
        [this](std::string path) { mouseBeginDrag(std::move(path)); });
    utils::AddFunctionToAnyRpc<void(std::string)>(methodManager, "mouseEndDrag", "End a drag with the mouse",
        [this](std::string path) { mouseEndDrag(std::move(path)); });
    utils::AddFunctionToAnyRpc<void(std::string, std::vector<std::string>)>(methodManager, "mouseDropUrls",
        "Drop Urls with mouse",
        [this](std::string path, std::vector<std::string> urls) { mouseDropUrls(std::move(path), urls); });
    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "inputText",
        "Enter text. Events are sent to the item's window at path.",
        [this](std::string path, std::string text) { inputText(std::move(path), std::move(text)); });
    utils::AddFunctionToAnyRpc<void(std::string, int, unsigned)>(methodManager, "enterKey", "Press and release a key",
        [this](std::string path, int keyCode, unsigned modifiers) { enterKey(std::move(path), keyCode, modifiers); });

    utils::AddFunctionToAnyRpc<std::string(std::string, std::string)>(methodManager, "getStringProperty",
        "Return a property as string", [this](std::string path, std::string property) {
            return getStringProperty(std::move(path), std::move(property));
        });
    utils::AddFunctionToAnyRpc<void(std::string, std::string, std::string)>(methodManager, "setStringProperty",
        "Set the string value of the given property",
        [this](std::string path, std::string property, std::string value) {
            setStringProperty(std::move(path), std::move(property), std::move(value));
        });
    utils::AddFunctionToAnyRpc<bool(std::string)>(methodManager, "existsAndVisible",
        "Returns true if the given object exists",
        [this](std::string path) { return existsAndVisible(std::move(path)); });
    utils::AddFunctionToAnyRpc<std::vector<std::string>()>(methodManager, "getErrors",
        "Returns internal errors that occurred during test execution", [this]() { return getErrors(); });

    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "takeScreenshot",
        "Take a screenshot of the object and save it as a file", [this](std::string targetItem, std::string filePath) {
            return takeScreenshot(std::move(targetItem), std::move(filePath));
        });

    utils::AddFunctionToAnyRpc<void()>(methodManager, "quit", "Close the app", [this] { quit(); });

    utils::AddFunctionToAnyRpc<void(std::string, std::string)>(methodManager, "command", "Executes a generic command",
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
