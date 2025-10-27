#pragma once
#include "OpenGL.h"
#include <algorithm>

namespace fc::gl {

	template<GLenum t_Type>
	class Buffer {
	protected:
		GLuint m_Handle;

		GLsizeiptr m_Size;
	public:
		Buffer() : m_Size(0) {
			glGenBuffers(1, &m_Handle);
		}
		~Buffer() {
			if(m_Handle != 0) {
				glDeleteBuffers(1, &m_Handle);
			}
		}

		// move assignment
		Buffer& operator=(Buffer&& other) {
			std::swap(m_Handle, other.m_Handle);
			std::swap(m_Size, other.m_Size);
			return *this;
		}
		
		// move constructor
		Buffer(Buffer&& other) {
			m_Handle = other.m_Handle;
			other.m_Handle = 0;
			m_Size = other.m_Size;
		}
		
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		inline void bind() const {
			glBindBuffer(t_Type, m_Handle);
		}

		inline void unbind() const {
			glBindBuffer(t_Type, 0);
		}

		void setData(const void* data) {
			setData(data, 0, m_Size);
		}

		void editData(const void* data, GLintptr offset, GLsizeiptr size) {
			bind();
			glBufferSubData(t_Type, offset, size, data);
			unbind();
		}

		void setData(const void* data, GLsizeiptr size, GLenum usage) {
			bind();
			glBufferData(t_Type, size, data, usage);
			m_Size = size;
			unbind();
		}

		void getData(void* dest) const {
			getData(dest, 0, m_Size);
		}

		void getData(void* dest, GLintptr offset, GLsizeiptr size) const {
			bind();
			glGetBufferSubData(t_Type, offset, size, dest);
			unbind();
		}

		void* dataPointer(GLbitfield access) {
			return dataPointer(0, m_Size, access);
		}

		void* dataPointer(GLintptr offset, GLsizeiptr size, GLbitfield access) {
			bind();
			void* ptr = glMapBufferRange(t_Type, offset, size, access);
			return ptr;
		}

		void close() {
			glUnmapBuffer(t_Type);
			unbind();
		}

		inline GLuint getHandle() const { return m_Handle; }
		inline GLsizeiptr getSize() const { return m_Size; }
	};
}