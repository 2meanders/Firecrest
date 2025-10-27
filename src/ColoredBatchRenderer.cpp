#include "ColoredBatchRenderer.h"
#include "res/ResourceManager.h"
#include "glm/gtc/matrix_transform.hpp"

static constexpr const char* VERTEX_SOURCE = R"(
#version 450 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in uint a_RGBA;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;

void main() {
	uint r =  a_RGBA        & 255;
	uint g = (a_RGBA >> 8)  & 255;
	uint b = (a_RGBA >> 16) & 255;
	uint a = (a_RGBA >> 24) & 255;
	v_Color = vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, float(a) / 255.0);
	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 0.0, 1.0);
}
)";


static constexpr const char* FRAGMENT_SOURCE = R"(
#version 450 core

layout (location=0) out vec4 o_Color;

in vec4 v_Color;

void main() {
	o_Color = v_Color;
}
)";

namespace fc {
	void ColoredBatchRenderer::createIndicesForQuads(size_t quadCount) {
		if (quadCount * INDICES_PER_QUAD == indices.size())
			return;
		indices.resize(quadCount * INDICES_PER_QUAD);

		uint32_t offset = 0;
		for (size_t i = 0; i < quadCount * INDICES_PER_QUAD; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		ibo.setIndices(indices.data(), static_cast<GLsizei>(indices.size()));
	}

	std::array<ColoredBatchRenderer::Vertex, 4> ColoredBatchRenderer::createQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color) {
		GLuint packedColor = ((GLuint)(GLubyte)(color.r * 255) << 0) |
			((GLuint)(GLubyte)(color.g * 255) << 8) |
			((GLuint)(GLubyte)(color.b * 255) << 16) |
			((GLuint)(GLubyte)(color.a * 255) << 24);

		ColoredBatchRenderer::Vertex v1;
		v1.position = glm::vec2(-0.5f, -0.5f) * scale + position;
		v1.RGBA = packedColor;

		ColoredBatchRenderer::Vertex v2;
		v2.position = glm::vec2(0.5f, -0.5f) * scale + position;
		v2.RGBA = packedColor;

		ColoredBatchRenderer::Vertex v3;
		v3.position = glm::vec2(0.5f, 0.5f) * scale + position;
		v3.RGBA = packedColor;

		ColoredBatchRenderer::Vertex v4;
		v4.position = glm::vec2(-0.5f, 0.5f) * scale + position;
		v4.RGBA = packedColor;

		return { v1, v2, v3, v4 };
	}

	ColoredBatchRenderer::ColoredBatchRenderer(Window& window, res::ResourceManager& resourceManager) : window(window), resourceManager(resourceManager) {		
		shader = resourceManager.loadShaderSource(VERTEX_SOURCE, FRAGMENT_SOURCE);
		shader->bind();

		vao.bind();
		vbo.bind();

		gl::VertexBufferLayout layout;
		layout.push(GL_FLOAT, 2); // Position
		//layout.push(GL_FLOAT, 4, GL_FALSE); // Color
		layout.push(GL_UNSIGNED_INT, 1); // Color

		vao.addBuffer(vbo, layout);
		vao.addBuffer(ibo);

		vao.unbind();
		vbo.unbind();
		ibo.unbind();
	}

	void ColoredBatchRenderer::clearElements() {
		vertices.clear();
	}

	void ColoredBatchRenderer::draw() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		vbo.setData(vertices.data(), vertices.size() * sizeof(ColoredBatchRenderer::Vertex), GL_STREAM_DRAW);

		shader->bind();

		const int width = window.width();
		const int height = window.height();
		const glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		const glm::mat4 projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);

		shader->setUniformMat4f("u_ViewProj", projection);
		shader->setUniformMat4f("u_Transform", view);
		vao.bind();
		ibo.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	void ColoredBatchRenderer::reserve(const size_t quadCount) {
		vertices.reserve(quadCount * VERTICES_PER_QUAD);
		createIndicesForQuads(quadCount);

		vbo.setData(nullptr, quadCount * sizeof(ColoredBatchRenderer::Vertex) * VERTICES_PER_QUAD, GL_STREAM_DRAW);
	}

	void ColoredBatchRenderer::addQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color) {
		std::array<ColoredBatchRenderer::Vertex, 4> quad = createQuad(position, scale, color);
		vertices.push_back(quad[0]);
		vertices.push_back(quad[1]);
		vertices.push_back(quad[2]);
		vertices.push_back(quad[3]);
	}
}