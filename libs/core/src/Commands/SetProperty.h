/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/ItemPath.h>

namespace spix {
namespace cmd {

class SetProperty : public Command {
public:
    SetProperty(ItemPath path, std::string propertyName, std::string propertyValue);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::string m_propertyName;
    std::string m_propertyValue;
};

} // namespace cmd
} // namespace spix
