#include "SearchItem.h"

#include <iostream>

#include <Scene/Scene.h>


namespace spix {
namespace cmd {

SearchItem::SearchItem(ItemPath path, bool first, std::promise<std::vector<std::string>> promise)
: m_path(std::move(path)),
m_first(first),
m_promise(std::move(promise))
{
}

void SearchItem::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);
    std::vector<std::string> completePath = {};

    if (!item) {
        env.state().reportError("SearchItem: Item not found: " + m_path.string());
        m_promise.set_value(completePath);
        return;
    }   
    m_promise.set_value(env.scene().searchEveryCompletePath(m_path));
    return;
}

} // namespace cmd
} // namespace spix