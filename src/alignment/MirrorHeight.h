#pragma once

#include "Element.h"

namespace fc::alignment {
    static AlignmentFunction MirrorHeight(const Element& element) {
        return [&element] (float parent1, float parent2) {return element.alignment.height(parent1, parent2);};
    }
}