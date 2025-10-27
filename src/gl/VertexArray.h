#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace fc::gl {

	class VertexBufferLayout;

	class VertexArray {
	private:
		GLuint m_Handle;
	public:
		VertexArray();
		~VertexArray();

		// move assignment
		VertexArray& operator=(VertexArray&& other);
		// move constructor
		VertexArray(VertexArray&& other);
		
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
		void addBuffer(const IndexBuffer& ib) const;

		void bind() const;
		void unbind() const;
	};

}