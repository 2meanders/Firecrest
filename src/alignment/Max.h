#pragma once
#include "Alignment.h"
#include <algorithm>

namespace fc::alignment {
	inline AlignmentFunction Max(AlignmentFunction func1, AlignmentFunction func2) {
		return [func1, func2](float parent1, float parent2) -> float {
			return std::max(func1(parent1, parent2), func2(parent1, parent2));
		};
	}
}
