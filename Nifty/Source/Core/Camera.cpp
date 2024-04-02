#include "nfpch.h"
#include "Camera.h"

namespace Nifty {

    void Camera::ProcessKeyboard(MOVEMENT_DIRECTION direction, float deltaTime, glm::vec3 front, glm::vec3 right)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += front * velocity;
        if (direction == BACKWARD)
            Position -= front * velocity;
        if (direction == LEFT)
            Position -= right * velocity;
        if (direction == RIGHT)
            Position += right * velocity;
    }
    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch -= yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }
    void Camera::ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 15.0f)
            Zoom = 15.0f;
        if (Zoom > 90.0f)
            Zoom = 90.0f;
    }

    void Camera::UpdateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
}