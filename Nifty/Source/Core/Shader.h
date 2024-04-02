#pragma once

namespace Nifty {

	class Shader
	{
	private:
		void CheckCompileErrors(unsigned int shader, std::string type);

	public:
		GLuint ID;

		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

		void Use();

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

