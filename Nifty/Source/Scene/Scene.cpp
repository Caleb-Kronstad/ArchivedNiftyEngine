#include "nfpch.h"

#include "Scene.h"

namespace Nifty
{
	Scene::Scene(const std::string& data_path, const std::string& name)
		: m_DataPath(data_path), m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	bool Scene::Load()
	{
		return true;
	}

	bool Scene::Save()
	{
		return true;
	}
}