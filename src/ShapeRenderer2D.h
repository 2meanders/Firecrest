#pragma once

#include "glm/glm.hpp"
#include "gl/VertexArray.h"
#include "gl/IndexBuffer.h"
#include "gl/Shader.h"
#include <vector>
#include <array>
#include "fiv.hpp"
#include "Window.h"
#include "Renderer.h"

namespace fc {

	class ShapeRenderer2D : public Renderer {
	private:
		struct Vertex {
			glm::vec2 position;
			GLuint RGBA; // A color packed into a uint (32 bit) in the format: RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA 
		};

		GLuint packColor(glm::vec4 color);
	private:
		gl::IndexBuffer m_IBO;
		gl::VertexBuffer m_VBO;
		gl::VertexArray m_VAO;
		gl::Shader m_Shader;
	public:
		ShapeRenderer2D();
		
		ShapeRenderer2D(const ShapeRenderer2D&) = delete;
		ShapeRenderer2D& operator=(const ShapeRenderer2D&) = delete;

		void beforeRender(const fc::Window& window) override;
		void afterRender(const Window& window) override;
		
		// Adds a shape with its vertices like a triangle fan
		// The vertices must be in a counterclockwise order
		void renderFan(const Window& window, const std::vector<Vertex>& vertices);
		void rect(const Window& window, glm::vec2 position, glm::vec2 scale, glm::vec4 color);
		void roundedRect(const Window& window, glm::vec2 position, glm::vec2 scale, glm::vec4 color, float radius, uint32_t quality);
		void circle(const Window& window, glm::vec2 center, float radius, glm::vec4 color, uint32_t quality = 64);

		void lineStrip(const Window& window, std::vector<glm::vec2> points, glm::vec4 color, float thickness);

		// Draws a line segment with a thickness of one.
		void lineSegment(const Window& window, glm::vec2 point1, glm::vec2 point2, glm::vec4 color);
	};
}
