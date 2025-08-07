/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class GetTestStatus : public Command {
public:
    using StatusStrings = std::vector<std::string>;

    GetTestStatus(bool errorsOnly, std::promise<StatusStrings> promise);

    void execute(CommandEnvironment& env) override;

private:
    std::promise<StatusStrings> m_promise;
};

} // namespace cmd
} // namespace spix
