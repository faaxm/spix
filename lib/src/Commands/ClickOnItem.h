/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPosition.h>
#include <future>

namespace spix {
namespace cmd {

class ClickOnItem : public Command {
public:
    ClickOnItem(ItemPosition path, std::promise<void> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPosition m_position;
    std::promise<void> m_promise;
};

} // namespace cmd
} // namespace spix
