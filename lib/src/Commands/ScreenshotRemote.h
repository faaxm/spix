#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class ScreenshotRemote : public Command {
public:
    ScreenshotRemote(ItemPath targetItemPath, std::promise<std::string> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_itemPath;
    std::promise<std::string> m_promise;
};

} // namespace cmd
} // namespace spix
