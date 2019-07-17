/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "InputText.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

InputText::InputText(ItemPath path, std::string text, std::promise<void> promise)
: m_path(std::move(path))
, m_text(std::move(text))
, m_promise(std::move(promise))
{
}

void InputText::execute(CommandEnvironment& env)
{
    try {
        auto item = env.scene().itemAtPath(m_path);

        if (item) {
            env.scene().events().stringInput(item.get(), m_text);
            m_promise.set_value();
        } else {
            throw std::runtime_error("InputText: Item not found: " + m_path.string());
        }
    } catch (const std::runtime_error& e) {
        env.state().reportError(e.what());
        m_promise.set_exception(std::make_exception_ptr(e));
    }
}

} // namespace cmd
} // namespace spix
