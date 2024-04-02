#pragma once

#include "Core/Shader.h"
#include "Core/Model.h"
#include "Core/GameObject.h"
#include "Core/Skybox.h"
#include "Core/Shadows.h"
#include "Core/Lighting.h"
#include "Core/Camera.h"

#include "Scene/SceneCamera.h"

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

		bool Load();
		bool Save();
	};
}