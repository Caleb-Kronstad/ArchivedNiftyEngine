#pragma once

#include "nfpch.h"

namespace Nifty {

	class Transform {
	public:
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 EulerAngles;
		glm::vec3 Rotation;
		float Angle;

		bool PhysicsActive = false;
		glm::vec3 Velocity = glm::vec3(0.0f);
		glm::vec3 Force = glm::vec3(0.0f);
		float Mass = 0.0f;

	public:
		Transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(1.0f), float angle = 1.0f,
			bool physicsActive = false, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 force = glm::vec3(0.0f), float mass = 0.0f)
			: Position(position), Scale(scale), Rotation(rotation), Angle(angle), PhysicsActive(physicsActive), Velocity(velocity), Force(force), Mass(mass)
		{
		}

		void AddPhysicsComponent(glm::vec3 velocity = glm::vec3(0.0), glm::vec3 force = glm::vec3(0.0), float mass = 0.0, bool physicsActive = true)
		{
			this->PhysicsActive = physicsActive;
			this->Velocity = velocity;
			this->Force = force;
			this->Mass = mass;
		}

		void SetPhysics(bool enabled) { PhysicsActive = enabled; }
	};
}