#pragma once
#include <string>
#include "glm/glm.hpp"
#include "gl/Shader.h"
#include <map>
#include "fiv.hpp"
#include "gl/VertexArray.h"
#include "gl/VertexBuffer.h"
#include "gl/Texture.h"
#include "Window.h"
#include <memory>

#include "ft2build.h"
#include FT_FREETYPE_H

namespace fc {
	class TextRenderer {
	private:
		struct Character {
			std::shared_ptr<gl::Texture> texture;
			glm::ivec2 size = { 0, 0 };
			glm::ivec2 bearing = { 0, 0 };
			uint32_t advance = 0;
		};

		struct Vertex {
			glm::vec3 pos;
			glm::vec2 texCoord;
		};
	private:
		static uint32_t _numInstances;
		static std::unique_ptr<gl::Shader> _textShader;
		static FT_Library _ftlib;
		gl::VertexArray _vao;
		gl::VertexBuffer _vbo;
		FT_Face _face;
		FT_UInt _nominalFontHeight;
	private:
		std::unordered_map<char, Character> _characters;
		void addChar(char charCode);
	public:
		TextRenderer(std::string fontPath, FT_UInt nominalFontHeight = 48);
		~TextRenderer();
		
		TextRenderer(const TextRenderer&) = delete;
		TextRenderer& operator=(const TextRenderer&) = delete;

		void renderText(const Window& window, const std::string& text, glm::vec3 pos, float scale, glm::vec4 color);
		float width(const std::string& text, float scale);
		float height(const std::string& text, float scale);
		float lineHeight(float scale);
		float descenderHeight(float scale);
		float ascenderHeight(float scale);
	};
}