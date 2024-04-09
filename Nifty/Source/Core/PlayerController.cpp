#include "nfpch.h"

#include "PlayerController.h"

namespace Nifty
{
    void PlayerController::ProcessKeyboard(Camera & camera, MOVEMENT_DIRECTION direction, float deltaTime)
    {
        float velocity = camera.MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

        UpdateVectors(camera);
    }

    void PlayerController::UpdateVectors(Camera & camera)
    {
        glm::vec3 front = glm::vec3(0.0f);
        front.x = cos(glm::radians(camera.Yaw));
        front.y = sin(glm::radians(camera.Pitch));
        front.z = sin(glm::radians(camera.Yaw));

        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
}