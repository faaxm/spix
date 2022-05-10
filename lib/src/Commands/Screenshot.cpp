/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "Screenshot.h"

#include <Scene/Scene.h>

namespace spix {
namespace cmd {

Screenshot::Screenshot(ItemPath targetItemPath, std::string filePath, std::promise<bool> promise)
: m_itemPath {std::move(targetItemPath)}
, m_filePath {std::move(filePath)}
, m_promise {std::move(promise)}
{
}

void Screenshot::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_itemPath);

    if (!item) {
        env.state().reportError("TakeScreenshot: Item not found: " + m_itemPath.string());
        m_promise.set_value(false);
        return;
    }

    bool isImageSaved = env.scene().takeScreenshot(*item, m_filePath);
    if (!isImageSaved) {
        env.state().reportError("TakeScreenshot: Couldn't save image at path: " + m_filePath
            + ". Did you forget to specify an image format ?");
    }
    m_promise.set_value(isImageSaved);
}

} // namespace cmd
} // namespace spix
