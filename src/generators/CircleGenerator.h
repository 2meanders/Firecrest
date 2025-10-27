#pragma once
#include "ShapeGenerator.h"
#include "core/Maths.h"

namespace fc {
	struct CircleGenerator : public ShapeGenerator {
		float radius;
		uint32_t quality;
		float da;

		CircleGenerator(float _radius, uint32_t _quality)
			: radius(_radius)
			, quality(_quality)
			, da((2 * maths::PI) / _quality)
		{}

		glm::vec2 getPoint(uint32_t i) const override {
			return radius * glm::vec2(cosf(da * i), sinf(da * i));
		}

		std::vector<glm::vec2> getPoints() const override {
			std::vector<glm::vec2> points;
			points.reserve(quality);
			for (uint32_t i = 0; i < quality; ++i) {
				points.push_back(getPoint(i));
			}
			return points;
		}
	};
}