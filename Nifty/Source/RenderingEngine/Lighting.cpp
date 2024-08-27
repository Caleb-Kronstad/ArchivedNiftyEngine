#include "nfpch.h"
#include "Lighting.h"

namespace Nifty {

	void LightingSystem::SetLighting(Shader& shader, float shininess)
	{
		std::vector<PointLight*>& pointLightsRef = *pointLights;
		std::vector<SpotLight*>& spotLightsRef = *spotLights;

		shader.SetInt("numPointLights", pointLightsRef.size());
		shader.SetInt("numSpotLights", spotLightsRef.size());

		shader.SetFloat("material.shininess", shininess);

		if (dirLight)
		{
			shader.SetVec3("dirLight.direction", dirLight->Position);
			shader.SetVec3("dirLight.ambient", dirLight->Color / 255.0f * 0.2f);
			shader.SetVec3("dirLight.diffuse", dirLight->Color / 255.0f);
			shader.SetVec3("dirLight.specular", dirLight->Color / 255.0f * 0.2f);
		}

		for (unsigned int i = 0; i < pointLightsRef.size(); i++)
		{
			std::string number = std::to_string(i);

			shader.SetVec3("pointLights[" + number + "].position", pointLightsRef[i]->Position);
			shader.SetVec3("pointLights[" + number + "].ambient", pointLightsRef[i]->Color / 255.0f * 0.1f);
			shader.SetVec3("pointLights[" + number + "].diffuse", pointLightsRef[i]->Color / 255.0f);
			shader.SetVec3("pointLights[" + number + "].specular", pointLightsRef[i]->Color / 255.0f);

			shader.SetFloat("pointLights[" + number + "].constant", 1.0f);
			shader.SetFloat("pointLights[" + number + "].linear", 0.09f);
			shader.SetFloat("pointLights[" + number + "].quadratic", 0.032f);
		}

		for (unsigned int i = 0; i < spotLightsRef.size(); i++)
		{
			std::string number = std::to_string(i);

			shader.SetVec3("spotLights[" + number + "].position", spotLightsRef[i]->Position);
			shader.SetVec3("spotLights[" + number + "].direction", spotLightsRef[i]->Direction);
			shader.SetVec3("spotLights[" + number + "].ambient", spotLightsRef[i]->Color / 255.0f * 0.1f);
			shader.SetVec3("spotLights[" + number + "].diffuse", spotLightsRef[i]->Color / 255.0f);
			shader.SetVec3("spotLights[" + number + "].specular", spotLightsRef[i]->Color / 255.0f * 0.5f);

			shader.SetFloat("spotLights[" + number + "].cutOff", glm::cos(glm::radians(12.5f)));
			shader.SetFloat("spotLights[" + number + "].outerCutOff", glm::cos(glm::radians(25.0f)));
			shader.SetFloat("spotLights[" + number + "].constant", 1.0f);
			shader.SetFloat("spotLights[" + number + "].linear", 0.09f);
			shader.SetFloat("spotLights[" + number + "].quadratic", 0.032f);
		}
	}
}