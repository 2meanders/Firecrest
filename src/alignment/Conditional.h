#pragma once
#include "Alignment.h"

namespace fc::alignment {
	// The parameter is the same as that of AlignmentFunction
	using ConditionalFunc = std::function<bool(float, float)>;
	
	inline AlignmentFunction Conditional(ConditionalFunc condition, AlignmentFunction ifTrue, AlignmentFunction ifFalse) {
		return [condition, ifTrue, ifFalse](float parent1, float parent2) -> float {
			if (condition(parent1, parent2)) {
				return ifTrue(parent1, parent2);
			}
			else {
				return ifFalse(parent1, parent2);
			}
		};
	}
}
