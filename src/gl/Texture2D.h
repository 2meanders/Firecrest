#pragma once

#include "Texture.h"

namespace fc::gl {

    class Texture2D : public Texture<GL_TEXTURE_2D> {
    private:
        int m_Width, m_Height;
        GLenum m_WrapMode = GL_CLAMP_TO_EDGE;
        GLenum m_MinMagFilter = GL_LINEAR;
    public:
        Texture2D();
        Texture2D(const std::string& path, bool blurred);
        Texture2D(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void* data);

        void setData(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void* data);

        inline int width() const { return m_Width; }
		inline int height() const { return m_Height; }
		
		glm::uvec3 size() const override;
    };
}
