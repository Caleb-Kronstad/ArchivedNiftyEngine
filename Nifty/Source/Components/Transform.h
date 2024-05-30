#pragma once

#include "nfpch.h"
#include "Components/Components.h"

namespace Nifty {

	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 EulerAngles; // converts to quaternion when drawing the object

		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 eulerAngles = glm::vec3(0.0f))
			: Position(position), Scale(scale), EulerAngles(eulerAngles)
		{
		}

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(EulerAngles / 180.0f * glm::pi<float>()));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};
}