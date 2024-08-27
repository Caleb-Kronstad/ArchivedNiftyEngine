#pragma once

#include "nfpch.h"
#include "RenderingEngine/Entity.h"
#include "RenderingEngine/Shaders/Shader.h"
#include "RenderingEngine/Components.h"

namespace Nifty 
{

	class LightingSystem
	{
	public:
		DirectionalLight* dirLight;
		std::vector<PointLight*>* pointLights;
		std::vector<SpotLight*>* spotLights;

		LightingSystem()
		{
			dirLight = nullptr;
			pointLights = new std::vector<PointLight*>;
			spotLights = new std::vector<SpotLight*>;
		}

		~LightingSystem(){}

		void SetLighting(Shader& shader, float shininess = 64.0f);
	};
}