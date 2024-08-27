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
			meshData->model->Draw(shader);

		PointLight* pnt_light = GetComponent<PointLight>();
		SpotLight* spt_light = GetComponent<SpotLight>();
		if (pnt_light)
			pnt_light->Position = transform.Position;
		if (spt_light)
			spt_light->Position = transform.Position;
	}

	Entity* Entity::FindWithName(const std::string& name, std::vector<Entity*>& entities)
	{
		for (unsigned int i = 0; i < entities.size(); ++i)
		{
			if (entities[i]->name == name)
				return entities[i];
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
			"\n        Rotation " + std::to_string(transform.EulerAngles.x) + " " + std::to_string(transform.EulerAngles.y) + " " + std::to_string(transform.EulerAngles.z) +
			"\n        HasShadows " + std::to_string(has_shadows) +
			"\n        ShaderID " + std::to_string(shader_id);

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
				data += 
					"\n        ModelID " + std::to_string(meshData->model_id);
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
			else if (m_Components[i]->type == ANIMATOR)
			{

			}
			else if (m_Components[i]->type == AUDIO)
			{

			}
			else if (m_Components[i]->type == DIR_LIGHT)
			{
				DirectionalLight* dirLight = GetComponent<DirectionalLight>();
				data +=
					"\n        Direction " + std::to_string(dirLight->Position.x) + " " + std::to_string(dirLight->Position.y) + " " + std::to_string(dirLight->Position.z) +
					"\n        Color " + std::to_string(dirLight->Color.x) + " " + std::to_string(dirLight->Color.y) + " " + std::to_string(dirLight->Color.z);
			}
			else if (m_Components[i]->type == PNT_LIGHT)
			{
				PointLight* pointLight = GetComponent<PointLight>();
				data +=
					"\n        Range " + std::to_string(pointLight->Scale.x) + " " + std::to_string(pointLight->Scale.y) + " " + std::to_string(pointLight->Scale.z) + 
					"\n        Color " + std::to_string(pointLight->Color.x) + " " + std::to_string(pointLight->Color.y) + " " + std::to_string(pointLight->Color.z);
			}
			else if (m_Components[i]->type == SPT_LIGHT)
			{
				SpotLight* spotLight = GetComponent<SpotLight>();
				data +=
					"\n        Direction " + std::to_string(spotLight->Direction.x) + " " + std::to_string(spotLight->Direction.y) + " " + std::to_string(spotLight->Direction.z);
					"\n        Range " + std::to_string(spotLight->Scale.x) + " " + std::to_string(spotLight->Scale.y) + " " + std::to_string(spotLight->Scale.z) +
					"\n        Color " + std::to_string(spotLight->Color.x) + " " + std::to_string(spotLight->Color.y) + " " + std::to_string(spotLight->Color.z);
			}
			else if (m_Components[i]->type == SKINNED_MESH)
			{
				SkinnedMesh* skinnedMesh = GetComponent<SkinnedMesh>();
				data +=
					"\n        ModelID " + std::to_string(skinnedMesh->model_id);
			}
			else
				Log::Error("Component type error during save");

			data += "\n      --C--";
		}

		data += "\n    --E--";

		saved = true;
		return data;
	}
}