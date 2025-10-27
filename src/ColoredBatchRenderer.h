#pragma once

#include "glm/glm.hpp"
#include "gl/VertexArray.h"
#include "gl/VertexBufferLayout.h"
#include "gl/IndexBuffer.h"
#include "res/ResourceManager.h"
#include <vector>
#include <array>
#include "fiv.hpp"
#include "Window.h"

namespace fc {

	class ColoredBatchRenderer {
	private:
		struct Vertex {
			glm::vec2 position;
			GLuint RGBA; // A color packed into a uint (32 bit) in the format: RRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA 
		};
	private:
		const uint32_t INDICES_PER_QUAD = 6;
		const uint32_t VERTICES_PER_QUAD = 4;
	private:
		std::vector<ColoredBatchRenderer::Vertex> vertices;

		std::vector<GLuint> indices;
		gl::IndexBuffer ibo;
		gl::VertexBuffer vbo;
		gl::VertexArray vao;
		res::ShaderHandle shader;

		res::ResourceManager& resourceManager;
		Window& window;
	private:
		void createIndicesForQuads(size_t quadCount);
		std::array<ColoredBatchRenderer::Vertex, 4> createQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color);
	public:
		ColoredBatchRenderer(Window& window, res::ResourceManager& resourceManager);
		void clearElements();
		void reserve(const size_t quadCount);
		void addQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color);
		void draw();
	};
}