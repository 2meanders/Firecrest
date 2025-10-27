#pragma once
#include "Alignment.h"

namespace fc::alignment {
	inline AlignmentFunction SwapRef(AlignmentFunction function) {
		return [function](float parent1, float parent2) { return function(parent2, parent1); };
	}
}
