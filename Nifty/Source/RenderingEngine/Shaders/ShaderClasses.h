#pragma once

#include "RenderingEngine/Shaders/Shader.h"
#include "RenderingEngine/Lighting.h"
#include "RenderingEngine/Shadows.h"

namespace Nifty
{

	class ShadowTextureLitShader : public Shader
	{
	private:

	public:
		void SetUniforms(glm::mat4& projection, glm::mat4& view, glm::vec3& view_pos, float& far_plane, glm::mat4& light_space_matrix, LightingSystem*& lighting, Shadows*& shadows)
		{
			SetMat4("projection", projection);
			SetMat4("view", view);
			SetVec3("viewPos", view_pos);
			//SetMat4("lightSpaceMatrix", light_space_matrix);
			SetFloat("far_plane", far_plane);
			lighting->SetLighting(*this);
			shadows->Draw();
		}

	};

	class SkinnedMeshShader : public Shader
	{
	private:

	public:

	};
}