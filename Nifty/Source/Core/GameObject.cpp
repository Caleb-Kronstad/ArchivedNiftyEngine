#include "nfpch.h"
#include "GameObject.h"

#include "Core/Log.h"

namespace Nifty {

	GameObject::GameObject(Transform _transform, Model* _model, std::string _name, unsigned int _id, unsigned int _modelid)
	{
		//collider.mainObject = this;

		type = "GameObject";

		name = _name;
		id = _id;
		model = _model;
		modelid = _modelid;
		transform = _transform;
	}

	void GameObject::Draw(Shader& shader, glm::mat4& matrix)
	{
		matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, transform.Position);
		matrix = glm::scale(matrix, transform.Scale);
		matrix = glm::rotate(matrix, glm::radians(transform.Angle), transform.EulerAngles);

		shader.SetMat4("model", matrix);
		if (model != nullptr)
			model->Draw(shader);
	}

	void GameObject::SetCollision(bool active)
	{
		colliderActive = active;
	}

	bool GameObject::CheckCollision(GameObject& collisionObject) const
	{
		//check for collision on the x-axis
		bool collisionX = this->transform.Position.x + this->transform.Scale.x >= collisionObject.transform.Position.x &&
			collisionObject.transform.Position.x + collisionObject.transform.Scale.x >= this->transform.Position.x;

		//check for collision on the y-axis
		bool collisionY = this->transform.Position.y + this->transform.Scale.y >= collisionObject.transform.Position.y &&
			collisionObject.transform.Position.y + collisionObject.transform.Scale.y >= this->transform.Position.y;

		//check for collision on the z-axis
		bool collisionZ = this->transform.Position.z + this->transform.Scale.z >= collisionObject.transform.Position.z &&
			collisionObject.transform.Position.z + collisionObject.transform.Scale.z >= this->transform.Position.z;

		//returns true only if objects are overlapping on all axis
		return collisionX && collisionY && collisionZ;
	}

	GameObject* GameObject::FindWithName(const std::string& name, std::vector<GameObject*>& objects)
	{
		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			if (objects[i]->name == name)
			{
				return objects[i];
			}
		}

		Log::Info("Could not find an object with name: " + name);
		return nullptr;
	}
}