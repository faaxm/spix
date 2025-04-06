/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "InvokeMethod.h"

#include <Spix/Scene/Scene.h>

namespace spix {
namespace cmd {

InvokeMethod::InvokeMethod(ItemPath path, std::string method, std::vector<Variant> args, std::promise<Variant> promise)
: m_path(std::move(path))
, m_method(std::move(method))
, m_args(std::move(args))
, m_promise(std::move(promise))
{
}

void InvokeMethod::execute(CommandEnvironment& env)
{
    auto item = env.scene().itemAtPath(m_path);

    if (item) {
        Variant ret;
        bool success = item->invokeMethod(m_method, m_args, ret);
        if (!success)
            env.state().reportError("InvokeMethod: Failed to invoke method: " + m_method);
        m_promise.set_value(ret);
    } else {
        env.state().reportError("InvokeMethod: Item not found: " + m_path.string());
        m_promise.set_value(Variant(nullptr));
    }
}

} // namespace cmd
} // namespace spix
