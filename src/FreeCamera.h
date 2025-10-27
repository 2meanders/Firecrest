#pragma once

#include "glm/glm.hpp"
#include "Camera.h"
#include "Window.h"

namespace fc {

    
    class FreeCamera : public Camera {
    public:
        struct MouseSensitivity {
            float pan;
            float tilt;

            MouseSensitivity(float sensitivity) 
                : pan(sensitivity)
                , tilt(sensitivity)
            {}

            MouseSensitivity& operator=(float sensitivity) {
                pan = sensitivity;
                tilt = sensitivity;
                return *this;
            }

            MouseSensitivity& operator=(const MouseSensitivity& other) = default;
        };
    public:
		FreeCamera();

		void pan(const float degrees);
		void tilt(const float degrees);
		void roll(const float degrees);
		void zoom(const float amount);
        
        /**
         * Set the projection matrix.
         * @param fov Field of view in radians.
         * @param zNear Near clipping plane distance.
         * @param zFar Far clipping plane distance.
         */
		void setProjection(const float fov, float zNear, float zFar);
		void localTranslate(const glm::vec3 translation);
		glm::mat4 localToWorld() const;
		
        virtual glm::mat4 viewMatrix() const override;
		virtual glm::mat4 projectionMatrix(const float aspectRatio) const override;
        virtual glm::vec3 getPosition() const override;

        virtual void handleInput(const Input& input, const Window& window, time::Duration delta) override;
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        float fovy;
        float zNear;
        float zFar;

        float movementSpeed;
        MouseSensitivity mouseSensitivity;

	};
}