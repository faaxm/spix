/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_export.h>

#include "Command.h"
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class SPIX_EXPORT GetProperty : public Command {
public:
    GetProperty(ItemPath path, std::string propertyName, std::promise<std::string> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::string m_propertyName;
    std::promise<std::string> m_promise;
};

} // namespace cmd
} // namespace spix
