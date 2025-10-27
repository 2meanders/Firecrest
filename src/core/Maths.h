#pragma once
#include <numbers>

namespace fc::maths {

	float map(float value, float fromMin, float fromMax, float toMin, float toMax);
	float abs(float value);
	
	float round(float n, int decimals);

	inline const float PI = std::numbers::pi_v<float>;
}