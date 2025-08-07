/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "SetProperty.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

SetProperty::SetProperty(ItemPath path, std::string propertyName, std::string propertyValue)
: m_path(std::move(path))
, m_propertyName(std::move(propertyName))
, m_propertyValue(std::move(propertyValue))
{
}

void SetProperty::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (item) {
        item->setStringProperty(m_propertyName, m_propertyValue);
    } else {
        env.state().reportError("SetProperty: Item not found: " + m_path.string());
    }
}

} // namespace cmd
} // namespace spix
