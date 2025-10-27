#pragma once
#include "Alignment.h"
#include "glm/glm.hpp"
#include "Pixels.h"
#include "Relative.h"

namespace fc::alignment {
    class ElementAlignment {
    public:
        AlignmentFunction x;
        AlignmentFunction y;
        AlignmentFunction width;
        AlignmentFunction height;

    public:
        ElementAlignment() : 
            x(Pixels(0)), 
            y(Pixels(0)), 
            width(Relative(1)), 
            height(Relative(1)) {}
        ElementAlignment(AlignmentFunction xAlignment, AlignmentFunction yAlignment, AlignmentFunction widthAlignment, AlignmentFunction heightAlignment)
            : x(xAlignment), y(yAlignment), width(widthAlignment), height(heightAlignment) {}
        
        glm::vec2 getPixelPosition(glm::vec2 parentSize) const {
            return {
                x(parentSize.x, parentSize.y),
                y(parentSize.y, parentSize.x)
            };
        }
        glm::vec2 getPixelSize(glm::vec2 parentSize) const {
            return {
                width(parentSize.x, parentSize.y),
                height(parentSize.y, parentSize.x)
            };
        }

        ElementAlignment& setX(AlignmentFunction xAlignment) {
            x = xAlignment;
            return *this;
        }
        
        ElementAlignment& setY(AlignmentFunction yAlignment) {
            y = yAlignment;
            return *this;
        }

        ElementAlignment& setWidth(AlignmentFunction widthAlignment) {
            width = widthAlignment;
            return *this;
        }

        ElementAlignment& setHeight(AlignmentFunction heightAlignment) {
            height = heightAlignment;
            return *this;
        }
    };

}
