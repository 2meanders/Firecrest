#pragma once
#include "glm/glm.hpp"
#include "res/ResourceManager.h"

namespace fc::gl {

	struct Material {
		fc::res::TextureHandle diffuseTexture; // map_Kd / map_Ka
		fc::res::TextureHandle specularMap; // map_Ks
		fc::res::TextureHandle normalMap; // map_Bump
		float shininess; // Ns
		float transparency; // 1 (opaque) - 0 (invisible) // d/Tr

		glm::vec3 ambientColor; //Ka
		glm::vec3 diffuseColor; //Kd
		glm::vec3 specularColor; //Ks

		glm::vec4 overrideColor; // An override color with negative values means no override color

		Material() :
			diffuseTexture(nullptr),
			specularMap(nullptr),
			normalMap(nullptr),
			shininess(64),
			transparency(1),
			ambientColor(0.2, 0.2, 0.2),
			diffuseColor(0.7, 0.7, 0.7),
			specularColor(1, 1, 1),
			overrideColor(-1, -1, -1, -1)
		{}
	};

}
