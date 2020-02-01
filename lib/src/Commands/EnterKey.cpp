/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "EnterKey.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

EnterKey::EnterKey(ItemPath path, int keyCode, Events::KeyModifier mod)
: m_path(std::move(path))
, m_keyCode(keyCode)
, m_mod(mod)
{
}

void EnterKey::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (item) {
        env.scene().events().keyPress(item.get(), m_keyCode, m_mod);
        env.scene().events().keyRelease(item.get(), m_keyCode, m_mod);
    } else {
        env.state().reportError("EnterKey: Item not found: " + m_path.string());
    }
}

} // namespace cmd
} // namespace spix
