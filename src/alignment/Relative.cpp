#include "Relative.h"

fc::alignment::AlignmentFunction fc::alignment::Relative(float coefficient) {
    return [coefficient](float parent1, float parent2) -> float { return coefficient * parent1; };
}
