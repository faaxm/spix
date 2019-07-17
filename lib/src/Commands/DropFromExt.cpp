/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "DropFromExt.h"

#include <CommandExecuter/CommandEnvironment.h>
#include <Scene/Scene.h>
#include <stdexcept>

namespace spix {
namespace cmd {

DropFromExt::DropFromExt(ItemPath path, PasteboardContent content, std::promise<void> promise)
: m_path(std::move(path))
, m_content(std::move(content))
, m_promise(std::move(promise))
{
}

void DropFromExt::execute(CommandEnvironment& env)
{
	try {
		auto item = env.scene().itemAtPath(m_path);

		if (!item)
            throw std::runtime_error("DropFromExt: Item not found: " + m_path.string());

		auto size = item->size();
		Point midPoint(size.width / 2.0, size.height / 2.0);
		env.scene().events().extMouseDrop(item.get(), midPoint, m_content);
		m_promise.set_value();
	} catch (const std::runtime_error& e) {
        env.state().reportError(e.what());
        m_promise.set_exception(std::make_exception_ptr(e));
	}
}

} // namespace cmd
} // namespace spix
