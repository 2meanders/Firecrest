#pragma once
#include <string>
#include "OpenGL.h"
#include "glm/glm.hpp"

namespace fc::gl {

	template<GLenum Dimension>
	class Texture {
	protected:
		GLuint m_Handle;
	public:
		Texture() {
			glGenTextures(1, &m_Handle);
		}

		~Texture() {
			if(m_Handle != 0) {
				glDeleteTextures(1, &m_Handle);
			}
		}

		// move assignment
		Texture& operator=(Texture&& other) {
			std::swap(m_Handle, other.m_Handle);
			return *this;
		}
		// move constructor
		Texture(Texture&& other) {
			m_Handle = other.m_Handle;
			other.m_Handle = 0;
		}

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		void bind(size_t slot) const {
			glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
			glBindTexture(Dimension, m_Handle);
		}
		inline void bind() const {
			glBindTexture(Dimension, m_Handle);
		}
		static void unbind() {
			glBindTexture(Dimension, 0);
		}

		inline GLuint getHandle() const { return m_Handle; }

	protected:
		virtual glm::uvec3 size() const = 0;
	};

}
