#pragma once

#include "nfpch.h"

#include "Core/Entity.h"
#include "Core/Transform.h"
#include "Core/Model.h"

#include "Physics/Collision.h"
#include "Math/BasicMathOperations.h"

namespace Nifty {

	class GameObject : public Entity
	{
	public:
		Transform transform;
		Model* model = nullptr;
		unsigned int modelid;

		// collision
		GameObject* currentCollidingObject = nullptr;
		bool colliderActive = true;
		bool colliding = false;

	public:
		GameObject(Transform _transform, Model* _model = nullptr, std::string _name = "Game Object", unsigned int _id = 0, unsigned int _modelid = 0);

		void Draw(Shader& shader, glm::mat4& matrix);
		void SetCollision(bool active);

		bool CheckCollision(GameObject& collisionObject) const;

		static GameObject* FindWithName(const std::string& name, std::vector<GameObject*>& objects);
	};
}