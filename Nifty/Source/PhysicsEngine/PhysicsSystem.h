#pragma once

#include "nfpch.h"
#include "RenderingEngine/Components.h"
#include "RenderingEngine/Transform.h"
#include "RenderingEngine/Entity.h"

namespace Nifty
{
	class PhysicsSystem
	{
	public:
		void StepPhysics(float dt);
		void CheckCollisions(Entity* entity_1);
		void AddObject(Entity* entity);
		void RemoveObject(Entity* entity);
		bool EntityInSystem(Entity* entity);

	private:
		glm::vec3 m_Gravity = glm::vec3(0.0f, -9.81f, 0.0f); // m/s/s
		std::vector<Entity*> m_PhysicsEntities;
	};
}