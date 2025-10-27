#pragma once

#include "Element.h"

namespace fc::alignment {
    static AlignmentFunction MirrorWidth(const Element& element) {
        return [&element] (float parent1, float parent2) {return element.alignment.width(parent1, parent2);};
    }
}