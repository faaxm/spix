#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class ScreenshotAsBase64 : public Command {
public:
    ScreenshotAsBase64(ItemPath targetItemPath, std::promise<std::string> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_itemPath;
    std::promise<std::string> m_promise;
};

} // namespace cmd
} // namespace spix
