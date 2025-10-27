#include "Pixels.h"

fc::alignment::AlignmentFunction fc::alignment::Pixels(float pixels) {
    return [pixels](float parent1, float parent2) -> float { return pixels; };
}
