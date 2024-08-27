#pragma once

#include "nfpch.h"
#include "RenderingEngine/Shaders/Shader.h"

namespace Nifty {

	constexpr int MAX_BONE_INFLUENCE = 3;

	struct Texture {
		unsigned int ID;
		std::string Type;
		std::string Path;
	};

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		
		//glm::vec3 Tangent;
		//glm::vec3 Bitangent;

		std::array<int, MAX_BONE_INFLUENCE> m_BoneIDs;
		std::array<float, MAX_BONE_INFLUENCE> m_Weights;
	};

	class Mesh
	{
	private:
		unsigned int VBO, EBO;
		void SetupMesh();

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int VAO;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Shader& shader);
	};
}