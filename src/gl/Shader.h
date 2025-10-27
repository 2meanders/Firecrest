#pragma once
#include "OpenGL.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace fc::gl {

	std::string getShaderText(const std::string& filePath);

	class Shader {
	private:
		GLuint m_Handle;
		std::vector<GLuint> m_Stages;
		mutable std::unordered_map<std::string, int> m_UniformLocations;
	public:
		Shader();
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		// move assignment
		Shader& operator=(Shader&& other);
		// move constructor
		Shader(Shader&& other);

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		void addStage(const GLenum shaderType, const std::string& filePath);
		void addStageSource(const GLenum shaderType, const std::string& source);
		void link();

		void bind() const;
		void unbind() const;

		void setUniformSamplers(const std::string& name, GLsizei count, const GLint* value) const;

		void setUniform1f(const std::string& name, GLfloat v0) const;
		void setUniform2f(const std::string& name, GLfloat v0, GLfloat v1) const;
		void setUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2) const;
		void setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;

		void setUniform1i(const std::string& name, GLint v0) const;
		void setUniform2i(const std::string& name, GLint v0, GLint v1) const;
		void setUniform3i(const std::string& name, GLint v0, GLint v1, GLint v2) const;
		void setUniform4i(const std::string& name, GLint v0, GLint v1, GLint v2, GLint v3) const;

		void setUniform1ui(const std::string& name, GLuint v0) const;
		void setUniform2ui(const std::string& name, GLuint v0, GLuint v1) const;
		void setUniform3ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2) const;
		void setUniform4ui(const std::string& name, GLuint v0, GLuint v1, GLuint v2, GLuint v3) const;

		void setUniform1fv(const std::string& name, const GLfloat* v) const;
		void setUniform2fv(const std::string& name, const GLfloat* v) const;
		void setUniform3fv(const std::string& name, const GLfloat* v) const;
		void setUniform4fv(const std::string& name, const GLfloat* v) const;

		void setUniform1iv(const std::string& name, const GLint* v) const;
		void setUniform2iv(const std::string& name, const GLint* v) const;
		void setUniform3iv(const std::string& name, const GLint* v) const;
		void setUniform4iv(const std::string& name, const GLint* v) const;

		void setUniform1uiv(const std::string& name, const GLuint* v) const;
		void setUniform2uiv(const std::string& name, const GLuint* v) const;
		void setUniform3uiv(const std::string& name, const GLuint* v) const;
		void setUniform4uiv(const std::string& name, const GLuint* v) const;

		void setUniformMat2f(const std::string& name, const glm::mat2& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat3f(const std::string& name, const glm::mat3& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat2x3f(const std::string& name, const glm::mat2x3& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat3x2f(const std::string& name, const glm::mat3x2& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat2x4f(const std::string& name, const glm::mat2x4& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat4x2f(const std::string& name, const glm::mat4x2& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat3x4f(const std::string& name, const glm::mat3x4& matrix, const GLboolean transpose = GL_FALSE) const;
		void setUniformMat4x3f(const std::string& name, const glm::mat4x3& matrix, const GLboolean transpose = GL_FALSE) const;

		bool uniformExists(const std::string& name);

		GLuint handle() const;
	private:
		GLint getUniformLocation(const std::string& name, bool warn = true) const;
	};

}