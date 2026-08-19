#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class SPIXCORE_EXPORT DumpTree : public Command {
public:
    DumpTree(ItemPath rootPath, std::promise<std::string> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_rootPath;
    std::promise<std::string> m_promise;
};

} // namespace cmd
} // namespace spix
