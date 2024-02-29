/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "ScreenshotRemote.h"

#include <Scene/Scene.h>
namespace spix {
namespace cmd {

ScreenshotRemote::ScreenshotRemote(ItemPath targetItemPath, std::promise<std::string> promise)
: m_itemPath {std::move(targetItemPath)}
, m_promise(std::move(promise))
{
}

void ScreenshotRemote::execute(CommandEnvironment& env)
{
    auto value = env.scene().takeScreenshotRemote(m_itemPath);
    m_promise.set_value(value);
}

} // namespace cmd
} // namespace spix