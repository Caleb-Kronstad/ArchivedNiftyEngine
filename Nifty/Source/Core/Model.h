#pragma once

#include "nfpch.h"
#include "Core/Mesh.h"
#include "Core/Shader.h"
#include "Core/Entity.h"

namespace Nifty {

	extern unsigned int LoadModelTexture(const char* path, bool flip = false);

	class Model : public Entity
	{
	public:
		std::vector<Mesh> meshes;
		std::string directory;
		std::string model_path;
		std::vector<Texture> textures_loaded;
		std::vector<unsigned int> textureTypeAmounts;

		Model(std::string path = "Assets/Models/Cube/Cube.obj", std::vector<unsigned int> textureTypeAmounts = { 0, 0, 0, 0 }, unsigned int _id = 0);

		void Draw(Shader& shader);
		std::vector<Texture> AddTextures(unsigned int amount, std::string type);

	private:
		void LoadModel(std::string const& path, std::vector<unsigned int>& textureTypeAmounts);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}