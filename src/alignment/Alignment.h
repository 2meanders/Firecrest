#pragma once
#include <functional>

namespace fc::alignment {
    // A function used to calculate the child's position or size based on the parent's width or height
    // The arguments are the parent's width and height in the following order:
    // if the alignment is used as x position or width:
    //  - parent width, parent height
    // if the alignment is used as y position or height:
    //  - parent height, parent width
    using AlignmentFunction = std::function<float(float, float)>;

}
