/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <string>
#include <vector>

namespace spix {

using CommandError = std::string;

class SPIXCORE_EXPORT ExecuterState {
public:
    using Errors = std::vector<CommandError>;

    void reportError(CommandError error);
    bool hasErrors() const;
    Errors errors() const;
    std::string errorsDescription() const;

private:
    std::vector<CommandError> m_errors;
};

} // namespace spix
