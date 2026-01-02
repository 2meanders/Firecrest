#include "Texture2D.h"
#include "stb_image.h"
#include <iostream>

fc::gl::Texture2D::Texture2D() : m_Width(0), m_Height(0)
{
}

fc::gl::Texture2D::Texture2D(const std::string &path, bool blurred) : m_Width(0), m_Height(0)
{
    stbi_set_flip_vertically_on_load(1);
	int bpp = 0;
	stbi_uc* imageData = stbi_load(path.c_str(), &m_Width, &m_Height, &bpp, 4);

	if (imageData == NULL) {
		std::cout << "Error loading texture: " << stbi_failure_reason() << ". Path: " << path << std::endl;
	}

	glGenTextures(1, &m_Handle);
	glBindTexture(GL_TEXTURE_2D, m_Handle);
	if (blurred) {
		m_MinMagFilter = GL_LINEAR;
	}
	else {
		//Sharp
		m_MinMagFilter = GL_NEAREST;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MinMagFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (imageData)
		stbi_image_free(imageData);
}

fc::gl::Texture2D::Texture2D(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void* data) : m_Width(width), m_Height(height) {  
	glGenTextures(1, &m_Handle);  
	setData(textureFormat, width, height, dataFormat, dataType, data);
}

void fc::gl::Texture2D::setData(GLint textureFormat, GLsizei width, GLsizei height, GLenum dataFormat, GLenum dataType, const void *data)
{	
	m_Width = width;
	m_Height = height;

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapMode);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapMode);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_MinMagFilter);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_MinMagFilter);  

	unbind();
}

glm::uvec3 fc::gl::Texture2D::size() const
{
    return {m_Width, m_Height, 1};
}
