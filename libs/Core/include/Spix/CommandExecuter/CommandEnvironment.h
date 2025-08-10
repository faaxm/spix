/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/spix_core_export.h>

#include <Spix/CommandExecuter/ExecuterState.h>

#include <string>
#include <vector>

namespace spix {

class Scene;

using CommandError = std::string;

class SPIXCORE_EXPORT CommandEnvironment {
public:
    CommandEnvironment(Scene& scene, ExecuterState& state);

    Scene& scene();
    ExecuterState& state();

private:
    Scene& m_scene;
    ExecuterState& m_state;
};

} // namespace spix
