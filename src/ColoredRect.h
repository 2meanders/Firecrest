#pragma once
#include "Element.h"
#include "ShapeRenderer2D.h"

namespace fc {

    class ColoredRect : public Element {
    public:
        glm::vec4 color;
		ShapeRenderer2D& renderer;
    public:
        ColoredRect(alignment::ElementAlignment alignment, glm::vec4 color, ShapeRenderer2D& renderer) : Element(alignment), color(color), renderer(renderer) {}
        ColoredRect(alignment::ElementAlignment alignment, glm::vec3 color, ShapeRenderer2D& renderer) : Element(alignment), color(color.x, color.y, color.z, 1.0f), renderer(renderer) {}

        void render(const Window& window, time::Duration delta) override {
            renderer.rect(window, getPixelPosition(), getPixelSize(), color);
        }
    };
}
