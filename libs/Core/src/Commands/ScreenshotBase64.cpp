/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "ScreenshotBase64.h"

#include <Spix/Scene/Scene.h>
namespace spix {
namespace cmd {

ScreenshotAsBase64::ScreenshotAsBase64(ItemPath targetItemPath, std::promise<std::string> promise)
: m_itemPath {std::move(targetItemPath)}
, m_promise(std::move(promise))
{
}

void ScreenshotAsBase64::execute(CommandEnvironment& env)
{
    auto value = env.scene().takeScreenshotAsBase64(m_itemPath);
    m_promise.set_value(value);
}

} // namespace cmd
} // namespace spix
