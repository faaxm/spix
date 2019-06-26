/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#include "CommandEnvironment.h"

namespace spix {

CommandEnvironment::CommandEnvironment(Scene& scene, ExecuterState& state)
: m_scene(scene)
, m_state(state)
{
}

Scene& CommandEnvironment::scene()
{
    return m_scene;
}

ExecuterState& CommandEnvironment::state()
{
    return m_state;
}

} // namespace uibot
