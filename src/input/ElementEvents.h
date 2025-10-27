#pragma once

#include "RawEvents.h"

namespace fc::input {
    using KeyAction = RawKeyAction;
    using Key = RawKey;
    using KeyboardEvent = RawKeyboardEvent;

    using MouseButton = RawMouseButton;

    using MouseButtonAction = RawMouseButtonAction;

    struct MouseButtonEvent {
        MouseButton button;
        MouseButtonAction action;
        int mods;
    };

    enum class MouseMotionAction {
        Move,
        Enter,
        Exit,
    };

    struct MouseMotionEvent {
        MouseMotionAction action;
        glm::vec2 position;
        glm::vec2 lastPosition;
    };

    using ScrollEvent = RawScrollEvent;

}