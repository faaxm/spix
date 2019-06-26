/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include "Command.h"
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class ExistsAndVisible : public Command {
public:
    ExistsAndVisible(ItemPath path, std::promise<bool> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::promise<bool> m_promise;
};

} // namespace cmd
} // namespace uibot
