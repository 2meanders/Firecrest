#include "RenderTarget.h"

fc::gl::RenderTarget::RenderTarget(int width, int height)
    : m_Width(width), m_Height(height) {
    create();
}

fc::gl::RenderTarget::~RenderTarget() {
    destroy();
}

void fc::gl::RenderTarget::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_Width, m_Height);
}

void fc::gl::RenderTarget::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fc::gl::RenderTarget::bindTexture(size_t slot) const
{
    m_colorTexture.bind(slot);
}

void fc::gl::RenderTarget::clear(float r, float g, float b, float a) const {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void fc::gl::RenderTarget::create()
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Color texture
    m_colorTexture.setData(GL_RGBA8, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_colorTexture.bind();

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_colorTexture.getHandle(),
        0
    );

    // Depth buffer
    glGenRenderbuffers(1, &m_depthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
    glRenderbufferStorage(
        GL_RENDERBUFFER,
        GL_DEPTH24_STENCIL8,
        m_Width,
        m_Height
    );

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        m_depthRbo
    );

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "fc::gl::RenderTarget framebuffer incomplete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fc::gl::RenderTarget::destroy()
{
    if (m_depthRbo) glDeleteRenderbuffers(1, &m_depthRbo);
    if (m_fbo) glDeleteFramebuffers(1, &m_fbo);

    m_depthRbo = 0;
    m_fbo = 0;
}

void fc::gl::RenderTarget::resize(int width, int height) {
    if (width == m_Width && height == m_Height) return;

    m_Width = width;
    m_Height = height;

    destroy();
    create();
}
