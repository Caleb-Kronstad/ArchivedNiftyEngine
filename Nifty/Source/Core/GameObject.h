#pragma once

#include "nfpch.h"
#include "Core/Entity.h"
#include "Core/Transform.h"
#include "Core/Model.h"

namespace Nifty {

	class GameObject : public Entity
	{
	public:
		Transform transform;
		std::string name;
		Model* model;
		unsigned int modelid;

		GameObject(Transform _transform, Model* _model, std::string _name = "Game Object", unsigned int _id = 0, unsigned int _modelid = 0);

		void Draw(Shader& shader, glm::mat4& matrix);
		void AddPhysicsComponent(glm::vec3 _velocity = glm::vec3(0.0), glm::vec3 _force = glm::vec3(0.0), float _mass = 0.0f, bool _active = true);
	};
}