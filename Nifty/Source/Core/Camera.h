#pragma once

#include "nfpch.h"

#include "Core/Entity.h"
#include "Core/Window.h"

namespace Nifty {

	enum MOVEMENT_DIRECTION {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera : public Entity
	{
	public:
		bool mouseLocked = false;

		bool firstMouse = false;
		float lastX = 0.0f;
		float lastY = 0.0f;

		bool flying = false;
		bool sprinting = false;

		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f)
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.1f), Zoom(60.0f)
		{
			type = "Camera";

			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			UpdateCameraVectors();
		}

		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5.0f), MouseSensitivity(0.1f), Zoom(60.0f)
		{
			type = "Camera";

			Position = glm::vec3(posX, posY, posZ);
			WorldUp = glm::vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			UpdateCameraVectors();
		}

		// returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix() const
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		glm::mat4 GetProjectionMatrix(Window& viewport) const
		{
			return glm::perspective(glm::radians(Zoom), (float)viewport.GetWidth() / (float)viewport.GetHeight(), 0.1f, 250.0f);
		}

		void ProcessKeyboard(MOVEMENT_DIRECTION direction, float deltaTime, glm::vec3 front, glm::vec3 right);
		void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
		void ProcessMouseScroll(float yoffset);

	private:
		void UpdateCameraVectors();
	};
}