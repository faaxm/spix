/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DragEnd.h"

#include <Scene/Scene.h>
#include <stdexcept>

namespace spix {
namespace cmd {

DragEnd::DragEnd(ItemPath path, std::promise<void> promise)
: m_path(std::move(path))
, m_promise(std::move(promise))
{
}

void DragEnd::execute(CommandEnvironment& env)
{
	try {
		auto item = env.scene().itemAtPath(m_path);

		if (!item)
            throw std::runtime_error("DragEnd: Item not found: " + m_path.string());

		auto size = item->size();
		Point midPoint(size.width / 2.0, size.height / 2.0);
		env.scene().events().mouseMove(item.get(), midPoint);
		env.scene().events().mouseUp(item.get(), midPoint, Events::MouseButtons::left);

		m_promise.set_value();
	} catch (const std::runtime_error& e) {
        env.state().reportError(e.what());
        m_promise.set_exception(std::make_exception_ptr(e));
	}
}

} // namespace cmd
} // namespace spix
