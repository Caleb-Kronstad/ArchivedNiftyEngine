#include "nfpch.h"
#include "GameObject.h"

#include "Core/Log.h"

namespace Nifty {

	GameObject::GameObject(Transform _transform, Model* _model, std::string _name, unsigned int _id, unsigned int _modelid)
	{
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
		model->Draw(shader);
	}

	void GameObject::AddPhysicsComponent(glm::vec3 _velocity, glm::vec3 _force, float _mass, bool _active)
	{
		transform.AddPhysicsComponent(_velocity, _force, _mass, _active);
	}
}