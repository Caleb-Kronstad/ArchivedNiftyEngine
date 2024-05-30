#pragma once

#include "Core/Model.h"

namespace Nifty
{
	enum COMPONENT_TYPE
	{
		NONE = 0,
		MESH = 1,
		PHYSICS = 2,
		COLLIDER = 3,
		ANIMATOR = 4,
		AUDIO = 5,
	};

	struct Component
	{
		bool enabled = true;
		COMPONENT_TYPE type = NONE;
		std::string name = "Component";

		virtual ~Component() {}
	};

	struct PhysicsBody : public Component
	{
		glm::vec3 Velocity = glm::vec3(0.0f);
		glm::vec3 Force = glm::vec3(0.0f);
		float Mass;

		PhysicsBody(float mass = 0.0f)
			: Mass(mass)
		{
			type = PHYSICS;
			name = "Physics Body";
		}
	};

	struct MeshData : public Component // mesh component
	{
		Model* model;
		unsigned int model_id;

		MeshData(Model* _model, unsigned int _id = 0)
			: model(_model), model_id(_id)
		{
			type = MESH;
			name = "Mesh Data";
		}
	};

	struct Animator : public Component // animator for animations
	{
		Animator()
		{
			type = ANIMATOR;
			name = "Animator";
		}
	};

	struct Collider : public Component
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		bool Colliding;

		Collider(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), bool colliding = false)
			: Position(position), Scale(scale), Colliding(colliding)
		{
			type = COLLIDER;
			name = "Collider";
		}
	};
}