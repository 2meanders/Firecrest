#pragma once
#include "glm/glm.hpp"
#include "Window.h"
#include "core/Time.h"

namespace fc {

	class Camera {
	public:
		virtual glm::vec3 getPosition() const = 0;
		virtual glm::mat4 viewMatrix() const = 0;
		virtual glm::mat4 projectionMatrix(const float aspectRatio) const = 0;

		virtual void handleInput(const Input& input, const Window& window, time::Duration delta) = 0;
	};
}