#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Defines options for camera movement. 
 * Intended as an abstraction to keep away from window/system specific input methods.
 * 
 */
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_UP_VECTOR = glm::vec3(0.0f, 1.0f, 0.0f);

const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;

const glm::vec3 DEFAULT_FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;

class Camera {

public:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(const glm::vec3 position = DEFAULT_POSITION, const glm::vec3 up = DEFAULT_UP_VECTOR, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    /**
     * @brief Returns a view matrix calculated using Euler angles and the LookAt matrix.
     * @return glm::mat4 
     */
    glm::mat4 getViewMatrix();

    /**
     * @brief Updates camera position based on keyboard input.
     * @param direction Direction of camera determined by input. Ex. 'W' translates to CameraMovement.FORWARD.
     * @param deltaTime 
     */
    void handleKeyboardInput(CameraMovement direction, float deltaTime);
    
    /**
     * @brief Updates camera rotation based on mouse input. Expects the offset value in both the x and y direction.
     * @param xoffset 
     * @param yoffset 
     * @param constrainPitch 
     */
    void handleMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    /**
     * @brief Updates camera zoom based on mouse scroll input.
     * @param yoffset 
     */
    void handleMouseScroll(float yoffset);


private:

    /**
     * @brief Calculates front, right, and up camera vectors from updated Euler angles.
     * 
     */
    void updateCameraVectors();

};

#endif