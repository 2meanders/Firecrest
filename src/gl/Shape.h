#pragma once

#include "VertexArray.h"
#include <vector>

namespace fc::gl {

template <typename T>
concept VertexType = requires {
    { T::layout() } -> std::same_as<fc::gl::VertexBufferLayout>;
};

template <VertexType VertexT>
class Shape {
public:
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;

    GLenum drawMode;

public:
    Shape& operator=(Shape&& other) noexcept;
    Shape(Shape&& other) noexcept;

    Shape(const Shape&) = delete;
    Shape& operator=(const Shape&) = delete;

    Shape();
    Shape(const std::vector<VertexT>& vertices, const std::vector<GLuint>& indices,
          GLenum drawMode_ = GL_TRIANGLES);

    void setVertices(const std::vector<VertexT>& vertices, const std::vector<GLuint>& indices,
                     GLenum drawMode = GL_TRIANGLES);
};
} // namespace fc::gl