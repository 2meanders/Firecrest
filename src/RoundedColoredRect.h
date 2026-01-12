#pragma once
#include "Element.h"
#include "ShapeRenderer2D.h"

namespace fc {

	class RoundedColoredRect : public Element {
	public:
		glm::vec4 color;
		float radius;
		ShapeRenderer2D& renderer;
		uint32_t quality;
	public:
		RoundedColoredRect(alignment::ElementAlignment alignment, glm::vec4 color, float radius, ShapeRenderer2D& renderer) : Element(alignment), color(color), radius(radius), renderer(renderer), quality(32) {}
		RoundedColoredRect(alignment::ElementAlignment alignment, glm::vec3 color, float radius, ShapeRenderer2D& renderer) : RoundedColoredRect(alignment, glm::vec4(color, 1.0f), radius, renderer) {}
		
		void render(const Window& window, time::Duration delta) override {
			renderer.roundedRect(window, getPixelPosition(), getPixelSize(), color, radius, quality);
		}
	};
}