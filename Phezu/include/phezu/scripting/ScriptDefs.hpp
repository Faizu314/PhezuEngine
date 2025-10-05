#pragma once

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
