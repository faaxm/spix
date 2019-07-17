/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "GetProperty.h"

#include <Scene/Scene.h>
#include <stdexcept>

namespace spix {
namespace cmd {

GetProperty::GetProperty(ItemPath path, std::string propertyName, std::promise<std::string> promise)
: m_path(std::move(path))
, m_propertyName(std::move(propertyName))
, m_promise(std::move(promise))
{
}

void GetProperty::execute(CommandEnvironment& env)
{
    try {
        auto item = env.scene().itemAtPath(m_path);

        if (item) {
            auto value = item->stringProperty(m_propertyName);
            m_promise.set_value(value);
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
