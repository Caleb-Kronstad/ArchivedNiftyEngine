#include "nfpch.h"
#include "Entity.h"

namespace Nifty
{
	Entity::Entity(Transform transform, std::string name, unsigned int id)
	{
		this->type = "Entity";
		this->name = name;
		this->id = id;
		this->transform = transform;
	}

	void Entity::Draw(Shader& shader, glm::mat4& matrix)
	{
		matrix = transform.GetTransformMatrix();

		shader.SetMat4("model", matrix);
		MeshData* meshData = GetComponent<MeshData>();
		if (meshData)
		{
			meshData->model->Draw(shader);
		}
	}

	Entity* Entity::FindWithName(const std::string& name, std::vector<Entity*>& entities)
	{
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			if (entities[i]->name == name)
			{
				return entities[i];
			}
		}

		Log::Info("Could not find an object with name: " + name);
		return nullptr;
	}

	std::string Entity::Save()
	{
		std::string data;

		data +=
			"\n    Entity " + std::to_string(id) +
			"\n      Type " + type +
			"\n      Name " + name +
			"\n      Transform " +
			"\n        Position " + std::to_string(transform.Position.x) + " " + std::to_string(transform.Position.y) + " " + std::to_string(transform.Position.z) +
			"\n        Scale " + std::to_string(transform.Scale.x) + " " + std::to_string(transform.Scale.y) + " " + std::to_string(transform.Scale.z) +
			"\n        Rotation " + std::to_string(transform.EulerAngles.x) + " " + std::to_string(transform.EulerAngles.y) + " " + std::to_string(transform.EulerAngles.z);

		for (unsigned int i = 0; i < m_Components.size(); ++i)
		{
			data +=
				"\n      Component "
				"\n        Type " + std::to_string(m_Components[i]->type) +
				"\n        Name " + m_Components[i]->name +
				"\n        Enabled " + std::to_string(m_Components[i]->enabled);

			if (m_Components[i]->type == NONE)
				Log::Warning("Unexpected component type of NONE when saving");
			else if (m_Components[i]->type == MESH)
			{
				MeshData* meshData = GetComponent<MeshData>();
				data += "\n\t    ModelID " + std::to_string(meshData->model_id);
			}
			else if (m_Components[i]->type == PHYSICS)
			{
				PhysicsBody* physicsBody = GetComponent<PhysicsBody>();
				data +=
					"\n        Velocity " + std::to_string(physicsBody->Velocity.x) + " " + std::to_string(physicsBody->Velocity.y) + " " + std::to_string(physicsBody->Velocity.z) +
					"\n        Force " + std::to_string(physicsBody->Force.x) + " " + std::to_string(physicsBody->Force.y) + " " + std::to_string(physicsBody->Force.z) +
					"\n        Mass " + std::to_string(physicsBody->Mass);
			}
			else if (m_Components[i]->type == COLLIDER)
			{
				Collider* collider = GetComponent<Collider>();
				data +=
					"\n        Position " + std::to_string(collider->Position.x) + " " + std::to_string(collider->Position.y) + " " + std::to_string(collider->Position.z) +
					"\n        Scale " + std::to_string(collider->Scale.x) + " " + std::to_string(collider->Scale.y) + " " + std::to_string(collider->Scale.z);
			}
			else {
				Log::Error("Component type error during save");
			}

			data += "\n      --C--";
		}

		data += "\n    --E--";

		saved = true;
		return data;
	}
}