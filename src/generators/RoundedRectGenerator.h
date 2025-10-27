#pragma once
#include "ShapeGenerator.h"
#include "CircleGenerator.h"

namespace fc {
	struct RoundedRectGenerator : public ShapeGenerator {
		glm::vec2 size;
		glm::vec2 centers[4];
		uint32_t arcQuality;
		CircleGenerator const circleGen;
		RoundedRectGenerator(glm::vec2 _size, float radius, uint32_t quality)
			: size(_size)
			, centers{
				{size.x - radius, size.y - radius}, // bottom right
				{radius, size.y - radius}, // bottom left
				{radius, radius}, // top left
				{size.x - radius, radius} // top right
			}
			, arcQuality(quality / 4)
			, circleGen(radius, quality - 4)
		{
		}

		glm::vec2 getPoint(uint32_t i) const override {
			const uint32_t corderIndex = i / arcQuality;
			return centers[corderIndex] + circleGen.getPoint(i - corderIndex);
		}

		std::vector<glm::vec2> getPoints() const override {
			const uint32_t quality = arcQuality * 4;
			std::vector<glm::vec2> points;
			points.reserve(quality);
			for (uint32_t i = 0; i < quality; ++i) {
				points.push_back(getPoint(i));
			}
			return points;
		}
	};
}