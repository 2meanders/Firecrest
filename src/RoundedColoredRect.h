#pragma once
#include "Element.h"
#include "ShapeRenderer2D.h"

namespace fc {

	class RoundedColoredRect : public Element {
	public:
		glm::vec4 m_Color;
		float m_Radius;
		ShapeRenderer2D& m_Renderer;
		uint32_t m_Quality;
	public:
		RoundedColoredRect(alignment::ElementAlignment alignment, glm::vec4 color, float radius, ShapeRenderer2D& renderer) : Element(alignment), m_Color(color), m_Radius(radius), m_Renderer(renderer), m_Quality(32) {}
		RoundedColoredRect(alignment::ElementAlignment alignment, glm::vec3 color, float radius, ShapeRenderer2D& renderer) : RoundedColoredRect(alignment, glm::vec4(color, 1.0f), radius, renderer) {}
		
		void render(const Window& window, time::Duration delta) override {
			m_Renderer.roundedRect(window, getPixelPosition(), getPixelSize(), m_Color, m_Radius, m_Quality);
		}
	};
}