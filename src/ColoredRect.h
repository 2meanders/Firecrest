#pragma once
#include "Element.h"
#include "ShapeRenderer2D.h"

namespace fc {

    class ColoredRect : public Element {
    public:
        glm::vec4 m_Color;
		ShapeRenderer2D& m_Renderer;
    public:
        ColoredRect(alignment::ElementAlignment alignment, glm::vec4 color, ShapeRenderer2D& renderer) : Element(alignment), m_Color(color), m_Renderer(renderer) {}
        ColoredRect(alignment::ElementAlignment alignment, glm::vec3 color, ShapeRenderer2D& renderer) : Element(alignment), m_Color(color.x, color.y, color.z, 1.0f), m_Renderer(renderer) {}

        void render(const Window& window, time::Duration delta) override {
            m_Renderer.rect(window, getPixelPosition(), getPixelSize(), m_Color);
        }
    };
}
