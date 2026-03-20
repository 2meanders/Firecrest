#pragma once
#include "glm/glm.hpp"
#include <vector>

namespace fc {
struct ShapeGenerator {
    virtual glm::vec2 getPoint(uint32_t i) const = 0;
    virtual std::vector<glm::vec2> getPoints() const = 0;
};
} // namespace fc