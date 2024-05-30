#include "nfpch.h"
#include "Model.h"

#include "Core/Log.h"
#include "Math/Math.h"

namespace Nifty {

	Model::Model(std::string path, std::vector<unsigned int> textureTypeAmounts, unsigned int _id)
	{
		type = "Model";
		model_path = path;
		id = _id;
		LoadModel(path, textureTypeAmounts);
	}

	Model::Model()
	{
		type = "Model";
		model_path = "";
		id = 0;
	}

	std::string Model::Save()
	{
		std::string data =
			"\n    Model " + std::to_string(id) +
			"\n      Name " + name +
			"\n      Path " + model_path +
			"\n      Textures " + std::to_string(textureTypeAmounts[0]) + " " + std::to_string(textureTypeAmounts[1]) + " " + std::to_string(textureTypeAmounts[2]) + " " + std::to_string(textureTypeAmounts[3]) + 
			"\n    --M--";

		saved = true;
		return data;
	}

	void Model::Draw(Shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}

	void Model::LoadModel(std::string const& path, std::vector<unsigned int>& textureTypeAmounts)
	{
		Assimp::Importer import;
		const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		this->textureTypeAmounts = textureTypeAmounts;

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		// process node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;

			// process vertex positions, normals, and texture coordinates
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			if (mesh->HasNormals()) {
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// process material
		std::vector<Texture> diffuseMaps = AddTextures(textureTypeAmounts[0], "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = AddTextures(textureTypeAmounts[1], "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = AddTextures(textureTypeAmounts[2], "normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps = AddTextures(textureTypeAmounts[3], "height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Model::AddTextures(unsigned int amount, std::string type)
	{
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < amount; i++)
		{
			std::string path = std::string(type);
			path = directory + '/' + path + "_" + std::to_string(i) + ".png";

			unsigned int id = LoadModelTexture(path.c_str());
			if (id == 0) return textures; // if there is no texture at this path then return all previously stored textures

			bool already_added = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].Path.data(), path.c_str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					already_added = true;
					break;
				}
			}

			if (!already_added) {
				Texture texture;
				texture.ID = LoadModelTexture(path.c_str());
				texture.Type = type.c_str();
				texture.Path = path;

				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}

	unsigned int LoadModelTexture(const char* path, bool flip)
	{
		stbi_set_flip_vertically_on_load(flip);

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			/* errors at this point are likely not a big issue, it's just searching for textures that aren't there so they won't be applied at all,
				which is likely the result of the "texture type amount" parameter on the model being too high */
			std::cout << "Texture failed to load at path: " << path << std::endl;
			return 0;
		}
		stbi_image_free(data);

		return textureID;
	}
}