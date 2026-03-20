#pragma once
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "res/types.h"
#include <utility>

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

} // namespace fc::gl
