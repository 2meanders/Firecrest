#pragma once
#include <string>
#include <vector>
#include <array>
#include "Window.h"
#include "gl/VertexArray.h"
#include "gl/VertexBufferLayout.h"
#include "gl/IndexBuffer.h"
#include "res/ResourceManager.h"

namespace fc {

	class TexturedBatchRenderer {
	private:
		struct Vertex {
			glm::vec2 position;
			GLuint RGBA;
			GLuint texIDTexCoordIndex;
		};
	private:
		const uint32_t INDICES_PER_QUAD = 6;
		const uint32_t VERTICES_PER_QUAD = 4;
	private:
		std::vector<TexturedBatchRenderer::Vertex> vertices;
		res::ResourceManager& resourceManager;

		glm::mat4 m_View;
		glm::mat4 m_Projection;

		std::vector<res::TextureHandle> textures;
		std::vector<GLuint> indices;
		gl::IndexBuffer ibo;
		gl::VertexBuffer vbo;
		gl::VertexArray vao;
		res::ShaderHandle shader;
	private:
		void createIndicesForQuads(size_t quadCount);
		std::array<TexturedBatchRenderer::Vertex, 4> createQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color, const uint32_t textureID);
	public:
		TexturedBatchRenderer(Window& window, res::ResourceManager& resourceManager);
		void addTexture(const std::string textureFile, const bool blurred);
		void clearElements();
		void draw(const Window& window);
		void reserve(const size_t count);
		void addQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color, const uint32_t textureIndex);
	};
}
