#include "Camera.h"
#include "Debug.h"

Camera::Camera(const glm::vec3 position, const glm::vec3 up, float yaw, float pitch)
    : front(DEFAULT_FRONT_VECTOR), movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVITY), zoom(DEFAULT_ZOOM) 
{
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : front(DEFAULT_FRONT_VECTOR), movementSpeed(DEFAULT_SPEED), mouseSensitivity(DEFAULT_SENSITIVITY), zoom(DEFAULT_ZOOM)
{
    this->position = glm::vec3(posX, posY, posZ);
    this->worldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    this->updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::handleKeyboardInput(CameraMovement direction, float deltaTime)
{
    float velocity = this->movementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        this->position += this->front * velocity;
    else if (direction == CameraMovement::BACKWARD)
        this->position -= this->front * velocity;
    else if (direction == CameraMovement::LEFT)
        this->position -= this->right * velocity;
    else if (direction == CameraMovement::RIGHT)
        this->position += this->right * velocity;

    // this->position.y = 0.0f; // This one-liner keeps the player at ground level.
}

void Camera::handleMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    const float maxPitch = 89.0f;

    this->yaw += xoffset * this->mouseSensitivity;
    this->pitch += yoffset * this->mouseSensitivity;

    if (constrainPitch && this->pitch > maxPitch) {
        this->pitch = maxPitch;
    } else if (constrainPitch && this->pitch < -maxPitch) {
        this->pitch = -maxPitch;
    }

    this->updateCameraVectors();
}

void Camera::handleMouseScroll(float yoffset)
{
    this->zoom -= yoffset;
    if (this->zoom < 1.0f) {
        this->zoom = 1.0f;
    } else if (this->zoom > 45.0f) {
        this->zoom = 45.0f;
    }
}

void Camera::updateCameraVectors()
{
    glm::vec3 frontVector;
    frontVector.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    frontVector.y = sin(glm::radians(this->pitch));
    frontVector.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(frontVector);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}