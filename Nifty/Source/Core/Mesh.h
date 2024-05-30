#pragma once

#include "nfpch.h"
#include "Core/Shader.h"

namespace Nifty {

	constexpr int MAX_BONE_INFLUENCE = 4;

	struct Texture {
		unsigned int ID;
		std::string Type;
		std::string Path;
	};

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		int m_BoneIDs[MAX_BONE_INFLUENCE];
		float m_Weights[MAX_BONE_INFLUENCE];
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