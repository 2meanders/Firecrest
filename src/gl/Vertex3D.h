#pragma once
#include "glm/glm.hpp"

namespace fc::gl {
    struct Vertex3D {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;


        bool operator==(const Vertex3D& other) const {
            return position == other.position && texCoord == other.texCoord && normal == other.normal && tangent == other.tangent;
        }

        bool operator!=(const Vertex3D& other) const {
            return position != other.position || texCoord != other.texCoord || normal != other.normal || tangent != other.tangent;
        }
    };
}