/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "InputText.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

InputText::InputText(ItemPath path, std::string text)
: m_path(std::move(path))
, m_text(std::move(text))
{
}

void InputText::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (item) {
        env.scene().events().stringInput(item.get(), m_text);
    } else {
        env.state().reportError("InputText: Item not found: " + m_path.string());
    }
}

} // namespace cmd
} // namespace spix
