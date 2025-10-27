#include "VertexArray.h"
#include "VertexBufferLayout.h"

namespace fc::gl {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_Handle);
	}

	VertexArray::~VertexArray() {
		if(m_Handle != 0) {
			glDeleteVertexArrays(1, &m_Handle);
		}
	}

    VertexArray &VertexArray::operator=(VertexArray &&other) {
		std::swap(m_Handle, other.m_Handle);
		return *this;
    }

    VertexArray::VertexArray(VertexArray &&other) {
		m_Handle = other.m_Handle;
		other.m_Handle = 0;
    }

    void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const {
		bind();
		vb.bind();
		const auto& elements = layout.getElements();
		uint32_t offset = 0;
		for (GLuint i = 0; i < elements.size(); i++) {
			const VertexBufferElement& element = elements[i];
			glEnableVertexAttribArray(i);
			if (VertexBufferElement::isInteger(element.type)) {
				glVertexAttribIPointer(i, element.count, element.type, layout.getStride(), reinterpret_cast<const void*>(offset));
			}
			else {
				glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), reinterpret_cast<const void*>(offset));
			}
			offset += element.count * VertexBufferElement::sizeOfType(element.type);
		}
	}

	void VertexArray::addBuffer(const IndexBuffer& ib) const {
		ib.bind();
	}

	void VertexArray::bind() const {
		glBindVertexArray(m_Handle);
	}

	void VertexArray::unbind() const {
		glBindVertexArray(0);
	}

}
