#pragma once

#include "nfpch.h"

namespace Nifty {

	class Transform {
	public:
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 EulerAngles;
		glm::quat Rotation;

		bool PhysicsActive = false;
		glm::vec3 Velocity = glm::vec3(0.0f);
		glm::vec3 Force = glm::vec3(0.0f);
		float Mass = 0.0f;

	public:
		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 eulerAngles = glm::vec3(1.0f),
			bool physicsActive = false, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 force = glm::vec3(0.0f), float mass = 0.0f, glm::quat rotation = glm::quat())
			: Position(position), Scale(scale), EulerAngles(eulerAngles), PhysicsActive(physicsActive), Velocity(velocity), Force(force), Mass(mass), Rotation(rotation)
		{
		}
	};
}