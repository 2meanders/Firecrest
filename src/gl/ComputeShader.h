#pragma once
#include "Shader.h"
#include <iostream>

namespace fc::gl {

	class ComputeShader : public Shader {
	public:
		ComputeShader(const std::string& filePath) {
			addStage(GL_COMPUTE_SHADER, filePath);
			link();
		}

		// Run the shader
		void run(glm::uvec3 numWorkGroups) {
			if(numWorkGroups.x == 0 || numWorkGroups.y == 0 || numWorkGroups.z == 0) {
				std::cerr << "ComputeShader::run: numWorkGroups must be non-zero. Dimensions are: (" << numWorkGroups.x << ", " << numWorkGroups.y << ", " << numWorkGroups.z << ")" << std::endl;
				return;
			}

			bind();
			dispatch(numWorkGroups);
			wait();
			unbind();
		}

		void dispatch(glm::uvec3 numWorkGroups) {
			glDispatchCompute(static_cast<GLuint>(numWorkGroups.x), static_cast<GLuint>(numWorkGroups.y), static_cast<GLuint>(numWorkGroups.z));
		}

		void wait() {
			glMemoryBarrier(GL_ALL_BARRIER_BITS);
		}

		void wait(GLbitfield barriers) {
			glMemoryBarrier(barriers);
		}
	};

}