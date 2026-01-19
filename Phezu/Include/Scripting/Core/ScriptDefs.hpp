// Copyright (c) 2026 Phezu
// Licensed under the MIT License

#pragma once

#include "Scripting/Core/MonoDefs.hpp"

struct InputFields {
    MonoClassField *W, *A, *S, *D, *Space;
};

enum class PhysicsEventType {
    CollisionEnter,
    CollisionStay,
    CollisionExit
};

enum class ManagedType {
    None,
    Transform,
    Shape,
    Renderer,
    Physics,
    ScriptComponent
};
