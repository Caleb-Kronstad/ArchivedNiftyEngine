#pragma once

#include "nfpch.h"
#include "Core/GameObject.h"
#include "Core/Shader.h"

namespace Nifty {

	struct DirectionalLight
	{
		glm::vec3 Position;
		glm::vec3 Color;

		DirectionalLight(glm::vec3 position = glm::vec3(0), glm::vec3 color = glm::vec3(0.5))
			: Position(position), Color(color)
		{
		}
	};

	struct PointLight
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 Color;

		PointLight(glm::vec3 position = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(0.5))
			: Position(position), Scale(scale), Color(color)
		{
		}
	};

	struct SpotLight
	{
		glm::vec3 Position;
		glm::vec3 Direction;
		glm::vec3 Scale;
		glm::vec3 Color;

		SpotLight(glm::vec3 position = glm::vec3(0), glm::vec3 direction = glm::vec3(0), glm::vec3 scale = glm::vec3(1), glm::vec3 color = glm::vec3(0.5))
			: Position(position), Direction(direction), Scale(scale), Color(color)
		{
		}
	};

	class Lighting
	{
	public:
		DirectionalLight* dirLight;
		std::vector<PointLight*>* pointLights;
		std::vector<SpotLight*>* spotLights;

		Lighting(DirectionalLight* dirlight, std::vector<PointLight*>* pointlights, std::vector<SpotLight*>* spotlights)
			: dirLight(dirlight), pointLights(pointlights), spotLights(spotlights)
		{
		}
		Lighting()
		{
			dirLight = new DirectionalLight(glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f));

			pointLights = new std::vector<PointLight*>();
			pointLights->push_back(new PointLight(glm::vec3(-3.0f, 2.5f, -1.5f), glm::vec3(0.25f), glm::vec3(1.0f)));

			spotLights = new std::vector<SpotLight*>();
			spotLights->push_back(new SpotLight(glm::vec3(500), glm::vec3(500), glm::vec3(0.25f), glm::vec3(1.0f)));
		}

		~Lighting(){}

		void AddLighting(DirectionalLight* dirlight, std::vector<PointLight*>* pointlights, std::vector<SpotLight*>* spotlights);
		void SetLighting(Shader& shader, float shininess = 64.0f);
		void DrawLightSources(Shader& shader, glm::mat4& matrix, Model& pointLightModel, Model& spotLightModel);
	};
}