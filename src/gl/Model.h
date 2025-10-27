#pragma once
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include <utility>
#include "res/types.h"
#include "Window.h"
#include "Camera.h"
#include "Light.h"

namespace fc::gl {

	class Model {
	public:
		// ModelID, Material
		std::vector<std::pair<fc::res::MeshHandle, gl::Material>> subMeshes;

		fc::res::ShaderHandle shader;

		glm::mat4 transform = glm::mat4(1.0f);

	public:
		Model() = default;

		void render(const Window& window, const Camera& camera, const Light& light);

		// move assignment
		Model& operator=(Model&& other);
		// move constructor
		Model(Model&& other);
		
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
	};

}
