#include "TexturedBatchRenderer.h"
#include "glm/gtc/matrix_transform.hpp"

static constexpr const char* VERTEX_SOURCE = R"(
#version 450 core
layout (location = 0) in vec2 a_Position;
layout (location = 1) in uint a_RGBA;
layout (location = 2) in uint a_TexIDTexCoordIndex;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out uint v_TexID;

void main() {
	uint r =  a_RGBA        & 255;
	uint g = (a_RGBA >> 8)  & 255;
	uint b = (a_RGBA >> 16) & 255;
	uint a = (a_RGBA >> 24) & 255;
	v_Color = vec4(float(r) / 255.0, float(g) / 255.0, float(b) / 255.0, float(a) / 255.0);
	
	vec2 texCoords[4] = {
		vec2(0.0, 0.0),
		vec2(1.0, 0.0),
		vec2(1.0, 1.0),
		vec2(0.0, 1.0),
	};
	uint texCoordIndex = a_TexIDTexCoordIndex & 3;
	uint texID = a_TexIDTexCoordIndex >> 2;
	v_TexCoord = texCoords[texCoordIndex];
	v_TexID = texID;

	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 0.0, 1.0);
}
)";

static constexpr const char* FRAGMENT_SOURCE = R"(
#version 450 core

layout (location=0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
flat in uint v_TexID;

uniform sampler2D u_Textures[16];

void main() {
	o_Color = texture(u_Textures[v_TexID], v_TexCoord) * v_Color;
}
)";

namespace fc {

	void TexturedBatchRenderer::createIndicesForQuads(const size_t quadCount) {
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

	std::array<TexturedBatchRenderer::Vertex, 4> TexturedBatchRenderer::createQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color, const uint32_t textureID) {
		GLuint packedColor = ((GLuint)(GLubyte)(color.r * 255) << 0) |
			((GLuint)(GLubyte)(color.g * 255) << 8) |
			((GLuint)(GLubyte)(color.b * 255) << 16) |
			((GLuint)(GLubyte)(color.a * 255) << 24);

		GLuint texID = textureID << 2;

		TexturedBatchRenderer::Vertex v1;
		v1.position = glm::vec2(-0.5f, -0.5f) * scale + position;
		v1.RGBA = packedColor;
		v1.texIDTexCoordIndex = texID | 0;

		TexturedBatchRenderer::Vertex v2;
		v2.position = glm::vec2(0.5f, -0.5f) * scale + position;
		v2.RGBA = packedColor;
		v2.texIDTexCoordIndex = texID | 1;

		TexturedBatchRenderer::Vertex v3;
		v3.position = glm::vec2(0.5f, 0.5f) * scale + position;
		v3.RGBA = packedColor;
		v3.texIDTexCoordIndex = texID | 2;

		TexturedBatchRenderer::Vertex v4;
		v4.position = glm::vec2(-0.5f, 0.5f) * scale + position;
		v4.RGBA = packedColor;
		v4.texIDTexCoordIndex = texID | 3;

		return { v1, v2, v3, v4 };
	}

	TexturedBatchRenderer::TexturedBatchRenderer(Window& window, res::ResourceManager& resourceManager) : resourceManager(resourceManager) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		int width = window.width();
		int height = window.height();
		shader = resourceManager.loadShaderSource(VERTEX_SOURCE, FRAGMENT_SOURCE);
		shader->bind();

		vao.bind();
		vbo.bind();

		gl::VertexBufferLayout layout;
		layout.push(GL_FLOAT, 2);
		layout.push(GL_UNSIGNED_INT, 1);
		layout.push(GL_UNSIGNED_INT, 1);
		// Padding 
		//layout.push(GL_FLOAT, 2, GL_FALSE);

		vao.addBuffer(vbo, layout);
		vao.addBuffer(ibo);

		vao.unbind();
		vbo.unbind();
		ibo.unbind();
	}

	void TexturedBatchRenderer::addTexture(const std::string textureFile, const bool blurred) {
		textures.push_back(resourceManager.loadTexture(textureFile, blurred));
	}

	void TexturedBatchRenderer::clearElements() {
		vertices.clear();
	}

	void TexturedBatchRenderer::reserve(const size_t count) {
		vertices.reserve(count * VERTICES_PER_QUAD);
		createIndicesForQuads(count);

		vbo.setData(nullptr, count * sizeof(TexturedBatchRenderer::Vertex) * VERTICES_PER_QUAD, GL_STREAM_DRAW);
	}

	void TexturedBatchRenderer::draw(const Window& window) {
		vbo.setData(vertices.data(), vertices.size() * sizeof(TexturedBatchRenderer::Vertex), GL_STREAM_DRAW);

		shader->bind();

		auto width = window.width();
		auto height = window.height();

		glm::mat4 projection = glm::ortho(static_cast<float>(-((int)width) / 2), static_cast<float>(width / 2), static_cast<float>(-((int)height) / 2), static_cast<float>(height / 2), -1.0f, 1.0f);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		std::vector<GLint> samplers;
		samplers.reserve(textures.size());
		for (size_t i = 0; i < textures.size(); i++) {
			samplers.push_back(static_cast<GLuint>(i));
		}
		shader->setUniformSamplers("u_Textures", static_cast<GLsizei>(textures.size()), samplers.data());

		for (size_t i = 0; i < textures.size(); i++) {
			textures[i]->bind(i);
		}

		shader->setUniformMat4f("u_ViewProj", projection);
		shader->setUniformMat4f("u_Transform", view);

		vao.bind();
		ibo.bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
	}

	void TexturedBatchRenderer::addQuad(const glm::vec2 position, const glm::vec2 scale, const glm::vec4 color, const uint32_t textureIndex) {
		std::array<TexturedBatchRenderer::Vertex, 4> quad = createQuad(position, scale, color, textureIndex);
		vertices.push_back(quad[0]);
		vertices.push_back(quad[1]);
		vertices.push_back(quad[2]);
		vertices.push_back(quad[3]);
	}

}