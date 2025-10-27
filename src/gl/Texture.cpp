#include "Texture.h"
#include "stb_image.h"
#include <iostream>

namespace fc::gl {
    Texture::Texture() {
		glGenTextures(1, &m_Handle);
    }

    Texture::Texture(const std::string& path, bool blurred) : m_Handle(0), m_Width(0), m_Height(0) {
		stbi_set_flip_vertically_on_load(1);
		int bpp = 0;
		stbi_uc* imageData = stbi_load(path.c_str(), &m_Width, &m_Height, &bpp, 4);

		if (imageData == NULL) {
			std::cout << "Error loading texture: " << stbi_failure_reason() << ". Path: " << path << std::endl;
		}

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);
		if (blurred) {
			//Blurred
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else {
			//Sharp
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (imageData)
			stbi_image_free(imageData);
	}

    Texture::Texture(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void* data) : m_Handle(0), m_Width(width), m_Height(height) {  
        glGenTextures(1, &m_Handle);  
		bind();

		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			textureFormat, 
			width, 
			height, 
			0, 
			dataFormat, 
			dataType, 
			data
		);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    }

	Texture::~Texture() {
		if(m_Handle != 0) {
			glDeleteTextures(1, &m_Handle);
		}
	}

    Texture &Texture::operator=(Texture &&other) {
        std::swap(m_Handle, other.m_Handle);
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		return *this;
    }

    Texture::Texture(Texture &&other) {
		m_Handle = other.m_Handle;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		other.m_Handle = 0;
		other.m_Width = 0;
		other.m_Height = 0;
    }

    void Texture::bind() const {
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}

	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    Texture Texture::clone() const {
		Texture tex;
		tex.m_Width = m_Width;
		tex.m_Height = m_Height;
		tex.bind();
		glCopyImageSubData(m_Handle, GL_TEXTURE_2D, 0, 0, 0, 0, tex.m_Handle, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);
		tex.unbind();
		return tex;
	}

    void Texture::bind(size_t slot) const {
		glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
		glBindTexture(GL_TEXTURE_2D, m_Handle);
	}
}
