/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include <Spix/CommandExecuter/ExecuterState.h>

namespace spix {

void ExecuterState::reportError(CommandError error)
{
    m_errors.emplace_back(std::move(error));
}

bool ExecuterState::hasErrors() const
{
    return m_errors.size() > 0;
}

ExecuterState::Errors ExecuterState::errors() const
{
    return m_errors;
}

std::string ExecuterState::errorsDescription() const
{
    std::string description;

    for (auto& err : m_errors) {
        if (description.length() > 0) {
            description += "\n";
        }
        description += err;
    }

    return description;
}

} // namespace spix
