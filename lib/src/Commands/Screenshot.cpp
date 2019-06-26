/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Screenshot.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

Screenshot::Screenshot(ItemPath targetItemPath, std::string filePath)
: m_itemPath {std::move(targetItemPath)}
, m_filePath {std::move(filePath)}
{
}

void Screenshot::execute(CommandEnvironment& env)
{
    env.scene().takeScreenshot(m_itemPath, m_filePath);
}

} // namespace cmd
} // namespace uibot
