#pragma once

#include "Core/Shader.h"
#include "Core/Model.h"
#include "Core/Entity.h"
#include "Core/Skybox.h"
#include "Core/Shadows.h"
#include "Core/Lighting.h"
#include "Core/Camera.h"

#include "Scene/SceneCamera.h"

#include "Components/PhysicsSystem.h"

namespace Nifty
{
	class Scene
	{
	private:
		std::string m_DataPath;
		std::string m_Name;

		Skybox* m_Skybox;

	public:
		Scene(const std::string& data_path, const std::string& name);
		~Scene();

		std::string& GetPath() { return m_DataPath; }
		std::string& GetName() { return m_Name; }

		bool Load(std::vector<Entity*>*& entities, std::vector<Model*>*& models, PhysicsSystem*& physics_system);
		bool Save(std::vector<Entity*>* entities, std::vector<Model*>* models);
	};
}