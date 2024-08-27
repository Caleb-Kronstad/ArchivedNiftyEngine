#include "nfpch.h"

#include "Shadows.h"

namespace Nifty
{
	void Shadows::Generate(const std::string& path)
	{
		map_texture = LoadTexture(path.c_str());

		glGenFramebuffers(1, &FBO);

		glGenTextures(1, &shadow_cubemap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_cubemap);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_cubemap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Shadows::LoadInScene(Shader& shader, glm::vec3& lightPos)
	{
		aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
		near_plane = 1.0f;
		far_plane = 25.0f;

		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);

		shadowTransforms.clear();

		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	}

	void Shadows::Set(Shader& shader, glm::vec3& lightPos)
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.Use();
		for (unsigned int i = 0; i < 6; ++i)
			shader.SetMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		shader.SetFloat("far_plane", far_plane);
		shader.SetVec3("lightPos", lightPos);
	}

	void Shadows::Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, map_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_cubemap);
	}

	void Shadows::Reset(bool cull) // unbinds the frame buffer and switches the cull setting to back faces (if cull is set to true)
	{
		if (cull) glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}