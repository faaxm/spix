#include "DumpTree.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

DumpTree::DumpTree(ItemPath rootPath, std::promise<std::string> promise)
: m_rootPath(std::move(rootPath))
, m_promise(std::move(promise))
{
}

void DumpTree::execute(CommandEnvironment& env)
{
    auto result = env.scene().dumpTree(m_rootPath);
    m_promise.set_value(std::move(result));
}

} // namespace cmd
} // namespace spix
