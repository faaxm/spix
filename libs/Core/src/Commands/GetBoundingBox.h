/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/Commands/Command.h>
#include <Spix/Data/Geometry.h>
#include <Spix/Data/ItemPath.h>

#include <future>

namespace spix {
namespace cmd {

class GetBoundingBox : public Command {
public:
    GetBoundingBox(ItemPath path, std::promise<Rect> promise);

    void execute(CommandEnvironment& env) override;

private:
    ItemPath m_path;
    std::promise<Rect> m_promise;
};

} // namespace cmd
} // namespace spix
