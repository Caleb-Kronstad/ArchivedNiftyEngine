#include "nfpch.h"

#include "PhysicsSystem.h"

namespace Nifty
{
	void PhysicsSystem::StepPhysics(float dt)
	{
		for (Entity* entity : m_PhysicsEntities)
		{
			PhysicsBody* physicsBody = entity->GetComponent<PhysicsBody>();
			Collider* collider = entity->GetComponent<Collider>();

			if (collider && collider->enabled) {
				CheckCollisions(entity);
			}

			if (physicsBody && physicsBody->enabled) {

				float mass = physicsBody->Mass;
				glm::vec3 force = physicsBody->Force + (mass * m_Gravity); // apply forces

				glm::vec3 velocity = physicsBody->Velocity + (force / physicsBody->Mass * dt);
				if (collider && collider->enabled && collider->Colliding)
				{
					velocity = glm::vec3(0.0f);
				}

				glm::vec3 position = entity->transform.Position + (velocity * dt);

				physicsBody->Force = force;
				physicsBody->Velocity = velocity;
				entity->transform.Position = position;
				physicsBody->Force = glm::vec3(0.0f); // reset force
			}
		}
	}

	void PhysicsSystem::CheckCollisions(Entity* entity_1)
	{
		Collider* collider = entity_1->GetComponent<Collider>();

		collider->Position = entity_1->transform.Position;
		collider->Scale = entity_1->transform.Scale;

		bool isColliding = false;
		for (Entity* entity_2 : m_PhysicsEntities)
		{
			Collider* collider_2 = entity_2->GetComponent<Collider>();

			if (entity_1->id != entity_2->id && collider_2) // make sure it's a different entity -- we don't want self collisions
			{
				glm::vec3 pos_1 = collider->Position;
				glm::vec3 pos_2 = collider_2->Position;
				glm::vec3 scale_1 = collider->Scale;
				glm::vec3 scale_2 = collider_2->Scale;

				bool collisionX = pos_1.x - scale_1.x <= pos_2.x + scale_2.x &&
					pos_1.x + scale_1.x >= pos_2.x - scale_2.x;

				bool collisionY = pos_1.y - scale_1.y <= pos_2.y + scale_2.y &&
					pos_1.y + scale_1.y >= pos_2.y - scale_2.y;

				bool collisionZ = pos_1.z - scale_1.z <= pos_2.z + scale_2.z &&
					pos_1.z + scale_1.z >= pos_2.z - scale_2.z;

				//true only if objects are overlapping on all axis
				if (collisionX && collisionY && collisionZ)
				{
					isColliding = true;
					break;
				}
			}
		}
		collider->Colliding = isColliding;
	}

	void PhysicsSystem::AddObject(Entity* entity)
	{
		m_PhysicsEntities.push_back(entity);
	}

	void PhysicsSystem::RemoveObject(Entity* entity)
	{
		if (!entity) return; // check if entity still exists
		auto itr = std::find(m_PhysicsEntities.begin(), m_PhysicsEntities.end(), entity); // find in vector
		if (itr == m_PhysicsEntities.end()) return; // check if entity is in vector
		m_PhysicsEntities.erase(itr); // remove from vector
	}

	bool PhysicsSystem::EntityInSystem(Entity* entity)
	{
		if (!entity) return false; // check if entity exists
		auto itr = std::find(m_PhysicsEntities.begin(), m_PhysicsEntities.end(), entity); // find in vector
		if (itr == m_PhysicsEntities.end()) return false;
		return true;
	}
}