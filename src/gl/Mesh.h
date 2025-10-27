#pragma once
#include <vector>
#include "gl/VertexArray.h"
#include "gl/VertexBuffer.h"
#include "gl/IndexBuffer.h"
#include "gl/Vertex3D.h"

namespace fc::gl {

	class Mesh {
	public:
		Mesh(const std::vector<Vertex3D>& vertices, const std::vector<GLuint>& indices);

		// move assignment
		Mesh& operator=(Mesh&& other);
		// move constructor
		Mesh(Mesh&& other);
		
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;
	public:
		VertexArray VAO;
		VertexBuffer VBO;
		IndexBuffer IBO;
	};

}