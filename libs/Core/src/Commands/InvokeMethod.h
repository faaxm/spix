/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>
#include <Spix/Data/Variant.h>

#include <future>

namespace spix {
namespace cmd {

class SPIXCORE_EXPORT InvokeMethod : public Command {
public:
    InvokeMethod(ItemPath path, std::string method, std::vector<Variant> args, std::promise<Variant> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::string m_method;
    std::vector<Variant> m_args;
    std::promise<Variant> m_promise;
};

} // namespace cmd
} // namespace spix
