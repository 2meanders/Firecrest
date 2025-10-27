#include "FreeCamera.h"
#include "glm/gtx/rotate_vector.hpp"

fc::FreeCamera::FreeCamera() 
    : up(0, 1, 0)
    , front(0, 0, -1)
    , position(0, 0, 1)
    , fovy(45)
    , zNear(0.1f)
    , zFar(100)
    , mouseSensitivity(3.4f)
    , movementSpeed(1.2f) 
{}

void fc::FreeCamera::pan(const float degrees) {
    front = glm::rotate(front, glm::radians(degrees), -up);
}

void fc::FreeCamera::tilt(const float degrees) {
    const glm::vec3 right = glm::cross(front, up);
    front = glm::rotate(front, glm::radians(degrees), right);
    // Rotating up axis as well allows for 360 rotations, but creates roll after tilt and pan movements (moving the mouse in circles).
    //up = glm::rotate(up, glm::radians(degrees), right);
}

void fc::FreeCamera::roll(const float degrees) {
    up = glm::rotate(up, glm::radians(degrees), -front);
}

void fc::FreeCamera::zoom(const float amount) {
    fovy -= amount;
}

void fc::FreeCamera::localTranslate(const glm::vec3 translation) {
    position -= glm::normalize(front) * translation.z;
    position += glm::normalize(up) * translation.y;
    const glm::vec3 left = glm::normalize(glm::cross(front, up));
    position += left * translation.x;
}

void fc::FreeCamera::setProjection(const float fov, float zNear, float zFar) {
    fovy = fov;
    zNear = zNear;
    zFar = zFar;
}

glm::mat4 fc::FreeCamera::localToWorld() const {
    return glm::inverse(glm::lookAt(position, position - front, up));
}

glm::mat4 fc::FreeCamera::viewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

glm::mat4 fc::FreeCamera::projectionMatrix(const float aspectRatio) const {
    return glm::perspective(fovy, aspectRatio, zNear, zFar);
}

glm::vec3 fc::FreeCamera::getPosition() const {
    return position;
}

void fc::FreeCamera::handleInput(const Input& input, const Window& window, time::Duration delta) {
    glm::vec2 mouseDelta = input.deltaMouse() * static_cast<float>(delta.seconds());

    // Calculate movement
    glm::vec3 movement{ 0, 0, 0 };
    if (input.keyPressed(GLFW_KEY_W))
        movement.z -= movementSpeed;
    if (input.keyPressed(GLFW_KEY_S))
        movement.z += movementSpeed;
    if (input.keyPressed(GLFW_KEY_A))
        movement.x -= movementSpeed;
    if (input.keyPressed(GLFW_KEY_D))
        movement.x += movementSpeed;
    if (input.keyPressed(GLFW_KEY_LEFT_SHIFT))
        movement.y -= movementSpeed;
    if (input.keyPressed(GLFW_KEY_SPACE))
        movement.y += movementSpeed;
    
    if (input.keyPressed(GLFW_KEY_F))
        movement *= 3;
        
    movement *= delta.seconds();
    mouseDelta.x *= mouseSensitivity.pan;
    mouseDelta.y *= mouseSensitivity.tilt;

    pan(mouseDelta.x);
    tilt(mouseDelta.y);
    localTranslate(movement);
}
