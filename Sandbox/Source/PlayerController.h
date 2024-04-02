#pragma once

#include "Nifty.h"

using namespace Nifty;

class PlayerController
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	PlayerController(Camera& camera, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
		: Front(glm::vec3(0.0f, 0.0f, -1.0f))
	{
		Position = position;
		WorldUp = up;

		UpdateVectors(camera);
	}

	void ProcessKeyboard(Camera& playerCamera, MOVEMENT_DIRECTION direction, float deltaTime);

private:
	void UpdateVectors(Camera& playerCamera);
};