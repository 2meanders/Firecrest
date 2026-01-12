#pragma once

#include "Element.h"
#include "gl/Shader.h"
#include "gl/VertexBuffer.h"
#include "gl/VertexArray.h"
#include "gl/IndexBuffer.h"
#include "gl/VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace fc {
    class ShaderQuad : public Element {
    public:
        gl::Shader shader;

        std::function<void(gl::Shader&)> beforeRender;
    private:
        gl::VertexArray m_VAO;
        gl::VertexBuffer m_VBO;
        gl::IndexBuffer m_IBO;

    public:
        /// @param alignment The alignment of the quad.
        /// @param shaderCode The fragment shader code. The fragment shader have the following inputs available:
        /// 
        /// - in vec2 uv; // The UV coordinates of the fragment, ranging from (0,0) at bottom-left to (1,1) at top-right.
        ///
        /// - in vec2 pixelPosition; // The pixel position of the fragment in screen space
        ShaderQuad(alignment::ElementAlignment alignment, const std::string& shaderCode, std::function<void(gl::Shader&)> beforeRender) : Element(alignment), beforeRender(beforeRender) {
            const std::string vertexShader = R"(
                #version 330 core
                layout(location = 0) in vec2 aPos;

                uniform mat4 transform;
                uniform mat4 projection;

                out vec2 uv;
                out vec2 pixelPosition;

                void main()
                {
                    uv = aPos.xy;
                    pixelPosition = (transform * vec4(aPos, 0.0, 1.0)).xy;
                    gl_Position = projection * transform * vec4(aPos, 0.0, 1.0);
                }
            )";

            shader.addStageSource(GL_VERTEX_SHADER, vertexShader);
            shader.addStageSource(GL_FRAGMENT_SHADER, shaderCode);
            shader.link();

            m_VAO.bind();

            gl::VertexBufferLayout layout;
            layout.push(GL_FLOAT, 2); // aPos

            m_VAO.addBuffer(m_VBO, layout);
            m_VAO.addBuffer(m_IBO);

            m_VAO.unbind();
            m_VBO.unbind();
            m_IBO.unbind();

            const float vertices[] = {
                0.0f,  0.0f, // bottom left
                1.0f,  0.0f, // bottom right
                1.0f,  1.0f, // top right
                0.0f,  1.0f  // top left 
            };

            GLuint indices[] = {
                0, 1, 2,
                2, 3, 0
            };

            m_VBO.setData(vertices, sizeof(vertices), GL_STATIC_DRAW);
            m_IBO.setIndices(indices, sizeof(indices) / sizeof(GLuint));
        }

        ShaderQuad(alignment::ElementAlignment alignment, const std::string& shaderCode) : ShaderQuad(alignment, shaderCode, nullptr) {}

        void render(const Window& window, time::Duration delta) override {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
		    
            shader.bind();

            if (beforeRender) {
                beforeRender(shader);
            }

            glm::mat4 projection = window.orthographicProjection();
            shader.setUniformMat4f("projection", projection);

            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(getPixelPosition(), 0.0f));
            transform = glm::scale(transform, glm::vec3(getPixelSize(), 1.0f));
            shader.setUniformMat4f("transform", transform);

            m_VAO.bind();
            m_IBO.bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_IBO.getCount()), GL_UNSIGNED_INT, nullptr);
            m_VAO.unbind();
            m_IBO.unbind();
        }
    };
}