#include "nfpch.h"
#include "Lighting.h"

namespace Nifty {

	void Lighting::AddLighting(DirectionalLight* dirlight, std::vector<PointLight*>* pointlights, std::vector<SpotLight*>* spotlights)
	{
		dirLight = dirlight;
		pointLights = pointlights;
		spotLights = spotlights;
	}

	void Lighting::SetLighting(Shader& shader, float shininess)
	{
		std::vector<PointLight*>& pointLightsRef = *pointLights;
		std::vector<SpotLight*>& spotLightsRef = *spotLights;

		shader.SetInt("numPointLights", pointLightsRef.size());
		shader.SetInt("numSpotLights", spotLightsRef.size());

		shader.SetFloat("material.shininess", shininess);
		shader.SetVec3("dirLight.direction", dirLight->Position);
		shader.SetVec3("dirLight.ambient", dirLight->Color * 0.2f);
		shader.SetVec3("dirLight.diffuse", dirLight->Color);
		shader.SetVec3("dirLight.specular", dirLight->Color * 0.2f);

		for (unsigned int i = 0; i < pointLightsRef.size(); i++)
		{
			std::string number = std::to_string(i);

			shader.SetVec3("pointLights[" + number + "].position", pointLightsRef[i]->Position);
			shader.SetVec3("pointLights[" + number + "].ambient", pointLightsRef[i]->Color * 0.1f);
			shader.SetVec3("pointLights[" + number + "].diffuse", pointLightsRef[i]->Color);
			shader.SetVec3("pointLights[" + number + "].specular", pointLightsRef[i]->Color);

			shader.SetFloat("pointLights[" + number + "].constant", 1.0f);
			shader.SetFloat("pointLights[" + number + "].linear", 0.09f);
			shader.SetFloat("pointLights[" + number + "].quadratic", 0.032f);
		}

		for (unsigned int i = 0; i < spotLightsRef.size(); i++)
		{
			std::string number = std::to_string(i);

			shader.SetVec3("spotLights[" + number + "].position", spotLightsRef[i]->Position);
			shader.SetVec3("spotLights[" + number + "].direction", spotLightsRef[i]->Direction);
			shader.SetVec3("spotLights[" + number + "].ambient", spotLightsRef[i]->Color * 0.1f);
			shader.SetVec3("spotLights[" + number + "].diffuse", spotLightsRef[i]->Color);
			shader.SetVec3("spotLights[" + number + "].specular", spotLightsRef[i]->Color * 0.5f);

			shader.SetFloat("spotLights[" + number + "].cutOff", glm::cos(glm::radians(12.5f)));
			shader.SetFloat("spotLights[" + number + "].outerCutOff", glm::cos(glm::radians(25.0f)));
			shader.SetFloat("spotLights[" + number + "].constant", 1.0f);
			shader.SetFloat("spotLights[" + number + "].linear", 0.09f);
			shader.SetFloat("spotLights[" + number + "].quadratic", 0.032f);
		}
	}

	void Lighting::DrawLightSources(Shader& shader, glm::mat4& matrix, Model& pointLightModel, Model& spotLightModel)
	{
		std::vector<PointLight*>& pointLightsRef = *pointLights;
		std::vector<SpotLight*>& spotLightsRef = *spotLights;

		for (unsigned int i = 0; i < pointLightsRef.size(); ++i)
		{
			matrix = glm::mat4(1.0f);
			matrix = glm::translate(matrix, pointLightsRef[i]->Position);
			matrix = glm::scale(matrix, pointLightsRef[i]->Scale);

			shader.SetMat4("model", matrix);
			shader.SetVec3("lightColor", pointLightsRef[i]->Color);
			pointLightModel.Draw(shader);
		}
		for (unsigned int i = 0; i < spotLightsRef.size(); ++i)
		{
			matrix = glm::mat4(1.0f);
			matrix = glm::translate(matrix, spotLightsRef[i]->Position);
			matrix = glm::scale(matrix, spotLightsRef[i]->Scale);

			shader.SetMat4("model", matrix);
			shader.SetVec3("lightColor", spotLightsRef[i]->Color);
			spotLightModel.Draw(shader);
		}
	}
}