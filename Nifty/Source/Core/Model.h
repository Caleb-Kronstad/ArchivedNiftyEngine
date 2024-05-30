#pragma once

#include "nfpch.h"
#include "Core/Mesh.h"
#include "Core/Shader.h"
#include "Core/SaveSystem.h"

namespace Nifty {

	extern unsigned int LoadModelTexture(const char* path, bool flip = false);

	enum MODEL_TYPE
	{
		OBJ,
		DAE
	};

	class Model : public SaveItem
	{
	public:
		std::vector<Mesh> meshes;
		std::string directory;
		std::string model_path;
		std::vector<Texture> textures_loaded;
		std::vector<unsigned int> textureTypeAmounts;

		Model(std::string path, std::vector<unsigned int> textureTypeAmounts = { 0, 0, 0, 0 }, unsigned int _id = 0);
		Model(); // initialize model without loading in

		void Draw(Shader& shader);
		std::string Save();
		void LoadModel(std::string const& path, std::vector<unsigned int>& textureTypeAmounts);

		std::vector<Texture> AddTextures(unsigned int amount, std::string type);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	};
}