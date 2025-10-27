#pragma once

#include "core/Rectangle.h"
#include <utility>

namespace fc::gl {

    class RenderRegion {
    public:
        enum class Mode : uint8_t {
            Viewport = 0b01,
            Scissor = 0b10,
            All = static_cast<uint8_t>(Viewport) | static_cast<uint8_t>(Scissor)
        };
    public:
        // Pushes a new render region with the given rectangle and mode.
        static void pushAbsolute(const Rectangle& region);
        static void pushAbsolute(const Rectangle& region, Mode mode);
        // Pushes a new render region that is the intersection with the current one.
        static void push(const Rectangle& region);
        static void push(const Rectangle& region, Mode mode);
        
        static void pop();

        static void applyBase();

        static Rectangle currentViewport();
        static Rectangle currentScissor();
    private:
        static inline std::vector<std::pair<Rectangle, Mode>> stack;
    public:
        static inline Rectangle base { 0, 0, 1, 1 }; 
    };

}
