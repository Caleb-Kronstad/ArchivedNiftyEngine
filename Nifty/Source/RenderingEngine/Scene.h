#pragma once

#include "RenderingEngine/Shaders/Shader.h"
#include "RenderingEngine/Model.h"
#include "RenderingEngine/Entity.h"
#include "RenderingEngine/Skybox.h"
#include "RenderingEngine/Shadows.h"
#include "RenderingEngine/Lighting.h"
#include "RenderingEngine/Camera.h"

#include "PhysicsEngine/PhysicsSystem.h"

namespace Nifty
{
	class Scene
	{
	private:
		std::string m_DataPath;
		std::string m_Name;

	public:
		Scene(const std::string& data_path, const std::string& name);
		~Scene();

		std::string& GetPath() { return m_DataPath; }
		std::string& GetName() { return m_Name; }

		bool Load(std::vector<Entity*>*& entities, std::vector<Model*>*& models, std::vector<Shader*>*& shaders, PhysicsSystem*& physics_system, LightingSystem*& lighting_system);
		bool LoadWithoutShaders(std::vector<Entity*>*& entities, std::vector<Model*>*& models, PhysicsSystem*& physics_system, LightingSystem*& lighting_system);
		bool Save(std::vector<Entity*>* entities, std::vector<Model*>* models, std::vector<Shader*>*& shaders);
	};
}