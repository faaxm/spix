/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <functional>

#include <Spix/Data/ItemPath.h>

namespace spix {

class CommandExecuter;

/**
 * @brief Base class that serves a test to the bot.
 *
 * A `TestServer` can be run by the bot and serves the
 * test to the bot. It can be a simple test case written
 * in C++ or it can be an actual server that handles
 * incoming RPC calls.
 * For basic C++ based tests, derive from this class and overide
 * the `executeTest` method.
 *
 * The code in `executeTest` is executed in its own thread so that
 * it does not affect the execution of the main application.
 */
class TestServer {
public:
    virtual ~TestServer();

    void start();
    void setCommandExecuter(CommandExecuter* exec);

    void setGenericCommandHandler(std::function<void(std::string, std::string)> handler);

    // Commands
    void wait(std::chrono::milliseconds waitTime);
    void mouseClick(ItemPath path);
    void mouseBeginDrag(ItemPath path);
    void mouseEndDrag(ItemPath path);
    void mouseDropUrls(ItemPath path, const std::vector<std::string>& urls);
    void genericCommand(std::string command, std::string payload);

    void inputText(ItemPath path, std::string text);

    std::string getStringProperty(ItemPath path, std::string propertyName);
    bool existsAndVisible(ItemPath path);
    std::vector<std::string> getErrors();

    void takeScreenshot(ItemPath targetItem, std::string filePath);
    void quit();

protected:
    virtual void executeTest() = 0;

private:
    CommandExecuter* m_cmdExec;
    std::thread m_thread;
    std::function<void(std::string, std::string)> m_handler;
};

} // namespace spix
