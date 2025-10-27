#pragma once
#include "glm/glm.hpp"

namespace fc {

	struct Light {
		glm::vec3 position = { 0, 0, 0 };

		glm::vec3 ambient = { 0.3f, 0.3f, 0.3f };
		glm::vec3 diffuse = { 0.6f, 0.6f, 0.6f };
		glm::vec3 specular = { 1.0f, 1.0f, 1.0f };
	};
}