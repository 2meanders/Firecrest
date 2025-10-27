#include "IndexBuffer.h"

namespace fc::gl {

	void IndexBuffer::setIndices(const GLuint* data, const GLsizei count, const GLenum usage) {
		setData(data, count * sizeof(GLuint), usage);
		m_Count = count;
	}
}
