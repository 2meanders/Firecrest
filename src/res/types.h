#pragma once

#include <memory>

namespace fc::gl {
    class Texture2D;
    class Shader;
    class Mesh;
    class Model;
}

namespace fc::res {
    using TextureHandle = std::shared_ptr<fc::gl::Texture2D>;
    using ShaderHandle  = std::shared_ptr<fc::gl::Shader>;
    using MeshHandle    = std::shared_ptr<fc::gl::Mesh>;
    using ModelHandle   = std::shared_ptr<fc::gl::Model>;
}
