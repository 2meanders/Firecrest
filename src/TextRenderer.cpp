#include "TextRenderer.h"
#include <stdexcept>
#include "gl/OpenGL.h"
#include "gl/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include "core/StringUtils.h"

static constexpr const char* VERTEX_SOURCE = 
R"(
#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
out vec2 TexCoords;

uniform mat4 projection;

void main() {
	gl_Position = projection * vec4(pos, 1.0);
	TexCoords = uv;
}
)";

static constexpr const char* FRAGMENT_SOURCE =
R"(
#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main() {
	float visible = texture(text, TexCoords).r;
	vec4 sampled = vec4(1.0, 1.0, 1.0, visible);
	color = textColor * sampled;
}
)";


void fc::TextRenderer::addChar(char charCode) {
	// Check if character already exists
	if (_characters.contains(charCode)) return;

	if (FT_Load_Char(_face, charCode, FT_LOAD_RENDER)) {
		throw std::exception(("Unable to load character with unicode code point: " + std::to_string(charCode)).c_str());
	}
	Character character = {
		std::make_shared<gl::Texture>(
			GL_RED,
			_face->glyph->bitmap.width,
			_face->glyph->bitmap.rows,
			GL_RED,
			GL_UNSIGNED_BYTE,
			_face->glyph->bitmap.buffer
		),
		glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
		glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
		static_cast<uint32_t>(_face->glyph->advance.x)
	};
	_characters.emplace(charCode, character);
}

fc::TextRenderer::TextRenderer(std::string fontPath, FT_UInt nominalFontHeight) : _nominalFontHeight(nominalFontHeight) {
	if(_numInstances == 0) {
		if (FT_Init_FreeType(&_ftlib)) {
			throw std::exception("Unable to initialize the FreeType Library");
		}
		_textShader = std::make_unique<gl::Shader>();
		_textShader->addStageSource(GL_VERTEX_SHADER, VERTEX_SOURCE);
		_textShader->addStageSource(GL_FRAGMENT_SHADER, FRAGMENT_SOURCE);
		_textShader->link();
	}
	_numInstances++;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	if (FT_New_Face(_ftlib, fontPath.c_str(), 0, &_face)) {
		throw std::exception("Unable to load font");
	}
	FT_Set_Pixel_Sizes(_face, 0, _nominalFontHeight);

	for(char c = 32; c < 127; c++) {
		addChar(c);
	}

	_vao.bind();
	gl::VertexBufferLayout layout;
	layout.push(GL_FLOAT, 3);
	layout.push(GL_FLOAT, 2);
	_vao.addBuffer(_vbo, layout);

	_vbo.bind();
	_vbo.setData(NULL, sizeof(TextRenderer::Vertex) * 6, GL_STATIC_DRAW);

	_vbo.unbind();
	_vao.unbind();
}

fc::TextRenderer::~TextRenderer() {
	_numInstances--;
	FT_Done_Face(_face);
	
	if (_numInstances == 0) {
		FT_Done_FreeType(_ftlib);
	}
}

void fc::TextRenderer::renderText(const Window& window, const std::string& text, glm::vec3 pos, float scale, glm::vec4 color) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glDisable(GL_DEPTH_TEST);

	glm::mat4 projection = window.orthographicProjection();
	_textShader->bind();
	_textShader->setUniform4f("textColor", color.x, color.y, color.z, color.w);
	_textShader->setUniformMat4f("projection", projection);

	_vao.bind();

	for (const auto& c : text) {
		if (!std::isprint(c)) continue; // Skip non-printable characters

		if (!_characters.contains(c)) {
			// This check is redundant
			addChar(c);
		}
		Character ch = _characters[c];

		float xpos = pos.x + ch.bearing.x * (scale / _nominalFontHeight);
		float ypos = pos.y - (ch.size.y - ch.bearing.y) * (scale / _nominalFontHeight);

		float w = ch.size.x * (scale / _nominalFontHeight);
		float h = ch.size.y * (scale / _nominalFontHeight);
		float z = pos.z;
		// update VBO for each character
		TextRenderer::Vertex vertices[6] = {
			{ { xpos,     ypos + h, z },  { 0.0f, 0.0f } },
			{ { xpos,     ypos,     z },  { 0.0f, 1.0f } },
			{ { xpos + w, ypos,     z },  { 1.0f, 1.0f } },

			{ { xpos,     ypos + h, z },  { 0.0f, 0.0f } },
			{ { xpos + w, ypos,     z },  { 1.0f, 1.0f } },
			{ { xpos + w, ypos + h, z },  { 1.0f, 0.0f } }
		};

		ch.texture->bind(0);

		assert(sizeof(vertices) == _vbo.getSize());
		_vbo.editData(vertices, 0, _vbo.getSize());
		_vbo.bind();
		_vao.bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);
		pos.x += (ch.advance >> 6) * (scale / _nominalFontHeight);
	}
	_vao.unbind();
	_vbo.unbind();
	gl::Texture::unbind();
}

float fc::TextRenderer::width(const std::string& text, float scale) {
	float x = 0;
	for (const auto& c : text) {
		if(!_characters.contains(c)) continue;

		x += (_characters[c].advance >> 6) * (scale / _nominalFontHeight);
	}
	return x;
}

float fc::TextRenderer::height(const std::string& text, float scale) {
	float minY = 1000.0f;
	float maxY = -1000.0f;
	for(const auto& c : text) {
		if(!_characters.contains(c)) continue;

		const Character& ch = _characters[c];
		float yMin = (ch.bearing.y - ch.size.y) * (scale / _nominalFontHeight);
		float yMax = ch.bearing.y * (scale / _nominalFontHeight);
		if (yMin < minY) {
			minY = yMin;
		}
		if (yMax > maxY) {
			maxY = yMax;
		}
	}
	return maxY - minY;
}

float fc::TextRenderer::lineHeight(float scale) {
	return (_face->size->metrics.height / 64.0f) * (scale / _nominalFontHeight);
}

float fc::TextRenderer::descenderHeight(float scale) {
    return (_face->size->metrics.descender / 64.0f) * (scale / _nominalFontHeight);
}

float fc::TextRenderer::ascenderHeight(float scale) {
    return (_face->size->metrics.ascender / 64.0f) * (scale / _nominalFontHeight);
}

uint32_t fc::TextRenderer::_numInstances = 0;
std::unique_ptr<fc::gl::Shader> fc::TextRenderer::_textShader;
FT_Library fc::TextRenderer::_ftlib;