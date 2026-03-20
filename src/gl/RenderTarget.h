#pragma once
#include "OpenGL.h"
#include "Texture2D.h"
#include <iostream>

namespace fc::gl {

class RenderTarget {
private:
    GLuint m_fbo = 0;
    GLuint m_depthRbo = 0;
    Texture2D m_colorTexture;

    int m_Width = 0;
    int m_Height = 0;

public:
    RenderTarget(int width, int height);
    ~RenderTarget();

    RenderTarget(const RenderTarget&) = delete;
    RenderTarget& operator=(const RenderTarget&) = delete;

    void bind() const;
    static void unbind();
    void bindTexture(size_t slot) const;

    void resize(int width, int height);
    void clear(float r, float g, float b, float a = 1.0f) const;

    Texture2D& texture() { return m_colorTexture; }
    int width() const { return m_Width; }
    int height() const { return m_Height; }

private:
    void create();
    void destroy();
};

} // namespace fc::gl
