#include "Shape.h"

template <fc::gl::VertexType VertexT>
fc::gl::Shape<VertexT>& fc::gl::Shape<VertexT>::operator=(Shape&& other) noexcept
{
    if (this != &other) {
        vao = std::move(other.vao);
        vbo = std::move(other.vbo);
        ibo = std::move(other.ibo);

        drawMode = other.drawMode;
    }

    return *this;
}

template <fc::gl::VertexType VertexT>
fc::gl::Shape<VertexT>::Shape(Shape&& other) noexcept
    : vao(std::move(other.vao)),
      vbo(std::move(other.vbo)),
      ibo(std::move(other.ibo)),
      drawMode(other.drawMode)
{
}

template <fc::gl::VertexType VertexT>
fc::gl::Shape<VertexT>::Shape() : drawMode(GL_TRIANGLES)
{
    vao.bind();

    VertexBufferLayout layout = VertexT::layout();
    vao.addBuffer(vbo, layout);
    vao.addBuffer(ibo);

    vao.unbind();
}

template <fc::gl::VertexType VertexT>
fc::gl::Shape<VertexT>::Shape(const std::vector<VertexT>& vertices,
                              const std::vector<GLuint>& indices, GLenum drawMode)
    : Shape()
{
    this->drawMode = drawMode;
    setVertices(vertices, indices);
}

template <fc::gl::VertexType VertexT>
void fc::gl::Shape<VertexT>::setVertices(const std::vector<VertexT>& vertices,
                                         const std::vector<GLuint>& indices, GLenum drawMode)
{
    this->drawMode = drawMode;
    vbo.setData(vertices.data(), sizeof(VertexT) * vertices.size(), GL_STATIC_DRAW);
    ibo.setIndices(indices.data(), indices.size());
}
