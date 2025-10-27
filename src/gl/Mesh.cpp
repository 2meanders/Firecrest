#include "Mesh.h"
#include "VertexBufferLayout.h"

namespace fc::gl {

	Mesh::Mesh(const std::vector<Vertex3D>& vertices, const std::vector<GLuint>& indices) {
		VBO.setData(nullptr, vertices.size() * sizeof(vertices[0]), GL_STATIC_DRAW);
		IBO.setData(nullptr, indices.size() * sizeof(indices[0]), GL_STATIC_DRAW);

		VertexBufferLayout layout;
		layout.push(GL_FLOAT, 3);
		layout.push(GL_FLOAT, 2);
		layout.push(GL_FLOAT, 3);
		layout.push(GL_FLOAT, 3);
		VBO.unbind();
		IBO.unbind();
		VAO.bind();

		VBO.bind();
		VAO.addBuffer(VBO, layout);
		VAO.addBuffer(IBO);

		VAO.unbind();
		VBO.setData(vertices.data(), vertices.size() * sizeof(vertices[0]), GL_STATIC_DRAW);
		IBO.setIndices(indices.data(), static_cast<GLsizei>(indices.size()));
		VBO.unbind();
		IBO.unbind();
	}

    Mesh &Mesh::operator=(Mesh &&other) {
        std::swap(VAO, other.VAO);
		std::swap(VBO, other.VBO);
		std::swap(IBO, other.IBO);
		return *this;
    }

    Mesh::Mesh(Mesh &&other) 
		: VAO(std::move(other.VAO))
		, VBO(std::move(other.VBO))
		, IBO(std::move(other.IBO))
	{}
}
