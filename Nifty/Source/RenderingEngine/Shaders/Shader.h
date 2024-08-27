#pragma once

#include "Core/FileUtility.h"

namespace Nifty 
{

	class Entity;

	class Shader : public SaveItem
	{
	private:
		GLuint m_ShaderID;
		std::vector<Entity*> m_Entities;

		void CheckCompileErrors(unsigned int shader, std::string type);

	public:
		std::string vPath = "None";
		std::string fPath = "None";
		std::string gPath = "None";

		Shader(unsigned int shaderID = 40000000, const std::string& shaderName = "Shader", 
			std::string vertexPath = "None", std::string fragmentPath = "None", std::string geometryPath = "None");

		std::string Save();
		void Load(unsigned int shaderID, const std::string& shaderName,
			std::string vertexPath, std::string fragmentPath, std::string geometryPath);
		void Use();

		std::vector<Entity*>& GetEntities() { return m_Entities; }
		void AddEntity(Entity* entity);
		void RemoveEntity(Entity* entity);

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;

		void SetMat2(const std::string& name, glm::mat2 value) const;
		void SetMat3(const std::string& name, glm::mat3 value) const;
		void SetMat4(const std::string& name, glm::mat4 value) const;

		void SetVec2(const std::string& name, float x, float y) const;
		void SetVec2(const std::string& name, glm::vec2 value) const;
		void SetVec3(const std::string& name, float x, float y, float z) const;
		void SetVec3(const std::string& name, glm::vec3 value) const;
		void SetVec4(const std::string& name, float x, float y, float z, float w) const;
		void SetVec4(const std::string& name, glm::vec4 value) const;
	};
}

