#pragma once

#include "RenderingEngine/Model.h"
#include "PhysicsEngine/Animation/Bone.h"
#include "PhysicsEngine/Animation/Animation.h"
#include "PhysicsEngine/Animation/Animator.h"

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
		DIR_LIGHT = 6,
		PNT_LIGHT = 7,
		SPT_LIGHT = 8,
		SKINNED_MESH = 9,
	};

	struct Component
	{
		bool enabled = true;
		COMPONENT_TYPE type = COMPONENT_TYPE::NONE;
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

	struct SkinnedMesh : public Component // skinned mesh component (more updated than other components)
	{
		
	private:

	public:
		Model* model;
		unsigned int model_id;

		SkinnedMesh(Model* _model, unsigned int _id = 0)
			: model(_model), model_id(_id)
		{
			type = SKINNED_MESH;
			name = "Skinned Mesh";
		}
		
	};

	struct AnimatorData : public Component // animator for animations
	{
		AnimatorData()
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


	struct DirectionalLight : public Component
	{
		glm::vec3 Position;
		glm::vec3 Color;

		DirectionalLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 color = glm::vec3(255.0f))
			: Position(position), Color(color)
		{
			type = DIR_LIGHT;
			name = "Directional Light";
		}
	};

	struct PointLight : public Component
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Scale;

		PointLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(255.0f))
			: Position(position), Color(color), Scale(scale)
		{
			type = PNT_LIGHT;
			name = "Point Light";
		}
	};

	struct SpotLight : public Component
	{
		glm::vec3 Position;
		glm::vec3 Color;
		glm::vec3 Direction;
		glm::vec3 Scale;

		SpotLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 direction = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 color = glm::vec3(255.0f))
			: Position(position), Color(color), Direction(direction), Scale(scale)
		{
			type = SPT_LIGHT;
			name = "Spot Light";
		}
	};
}