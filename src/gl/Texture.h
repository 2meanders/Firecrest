#pragma once
#include <string>
#include "OpenGL.h"

namespace fc::gl {

	class Texture {
	private:
		GLuint m_Handle;
		int m_Width, m_Height;
	public:
		Texture();
		Texture(const std::string& path, bool blurred);
		Texture(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void* data);
		~Texture();

		// move assignment
		Texture& operator=(Texture&& other);
		// move constructor
		Texture(Texture&& other);
		

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind(size_t slot) const;
		inline void bind() const;
		static void unbind();

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }
		inline GLuint getHandle() const { return m_Handle; }

		Texture clone() const;
	};

}
