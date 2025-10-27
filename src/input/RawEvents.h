#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

namespace fc::input {
    
    enum class RawKeyAction {
        Press,
        Repeat,
        Up,
    };
    
    using RawKey = int;
    
    struct RawKeyboardEvent {
        RawKey key;
        RawKeyAction action;
        int scancode;
        int mods; // See: https://www.glfw.org/docs/3.3/group__mods.html
    };
    
    enum class RawMouseButton {
        Button1 = GLFW_MOUSE_BUTTON_1,
        Button2 = GLFW_MOUSE_BUTTON_2,
        Button3 = GLFW_MOUSE_BUTTON_3,
        Button4 = GLFW_MOUSE_BUTTON_4,
        Button5 = GLFW_MOUSE_BUTTON_5,
        Button6 = GLFW_MOUSE_BUTTON_6,
        Button7 = GLFW_MOUSE_BUTTON_7,
        Button8 = GLFW_MOUSE_BUTTON_8,
        
        Left = GLFW_MOUSE_BUTTON_LEFT,
        Right = GLFW_MOUSE_BUTTON_RIGHT,
        Middle = GLFW_MOUSE_BUTTON_MIDDLE,
        
        Count
    };
    
    enum class RawMouseButtonAction {
        Press,
        Up,
    };
    
    struct RawMouseButtonEvent {
        RawMouseButton button;
        RawMouseButtonAction action;
        int mods;
    };
    
    struct RawMouseMotionEvent {
        glm::vec2 position;
        glm::vec2 lastPosition;
    };

    struct RawScrollEvent {
        glm::vec2 offset;
    };

    // According to the glfw docs, an unsigned int is 32 bit on all supported platforms. https://www.glfw.org/docs/3.3/input_guide.html
    // Encoded in UTF-32, native endian
    using UnicodeCodePoint = unsigned int;
}