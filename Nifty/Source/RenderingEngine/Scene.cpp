#include "nfpch.h"

#include "Scene.h"

namespace Nifty
{
	Scene::Scene(const std::string& data_path, const std::string& name)
		: m_DataPath(data_path), m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	bool Scene::Load(std::vector<Entity*>*& entities, std::vector<Model*>*& models, std::vector<Shader*>*& shaders, PhysicsSystem*& physics_system, LightingSystem*& lighting_system)
	{
		models = new std::vector<Model*>;
		entities = new std::vector<Entity*>;
		shaders = new std::vector<Shader*>;
		physics_system = new PhysicsSystem;
		lighting_system = new LightingSystem;

		std::string& path = GetPath();

		std::ifstream file(path);
		std::string line;

		bool on_model = false;
		bool on_entity = false;
		bool on_component = false;
		bool on_shader = false;
		float x, y, z;
		unsigned int id;
		unsigned int c_type = NONE;
		unsigned int enabled;
		std::string str;

		int shaderi;
		float shaderf;
		glm::mat2 shadermat2;
		glm::mat3 shadermat3;
		glm::mat4 shadermat4;
		glm::vec2 shadervec2;
		glm::vec3 shadervec3;
		glm::vec4 shadervec4;

		Entity* placeholder_entity = new Entity();
		MeshData* mesh_data = nullptr;
		SkinnedMesh* skinned_mesh = nullptr;
		PhysicsBody* physics_body = nullptr;
		Collider* collider = nullptr;
		AnimatorData* animator = nullptr;
		DirectionalLight* dir_light = nullptr;
		PointLight* point_light = nullptr;
		SpotLight* spot_light = nullptr;

		Model* placeholder_model = new Model();

		Shader* placeholder_shader = new Shader();

		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string type;

			if (iss >> type)
			{
				if (type == "Shader")
				{
					on_shader = true;
					if (iss >> id)
						placeholder_shader->id = id;
				}
				if (type == "--S--")
				{
					on_shader = false;
					Shader* new_shader = new Shader(placeholder_shader->id, placeholder_shader->name, placeholder_shader->vPath, placeholder_shader->fPath, placeholder_shader->gPath);
					shaders->push_back(new_shader);
					placeholder_shader = new Shader();
				}

				if (on_shader)
				{
					if (type == "Name")
					{
						if (iss >> str)
							placeholder_shader->name = str;
					}
					else if (type == "VertexPath")
					{
						if (iss >> str)
							placeholder_shader->vPath = str;
					}
					else if (type == "FragmentPath")
					{
						if (iss >> str)
							placeholder_shader->fPath = str;
					}
					else if (type == "GeometryPath")
					{
						if (iss >> str)
							placeholder_shader->gPath = str;
					}
				}

				if (type == "Model")
				{
					on_model = true;
					if (iss >> id)
						placeholder_model->id = id;
				}
				if (type == "--M--")
				{
					on_model = false;
					Model* new_model = new Model(*placeholder_model);
					new_model->LoadModel(new_model->model_path, new_model->textureTypeAmounts);
					models->push_back(new_model);
					placeholder_model = new Model();
				}

				if (on_model)
				{
					if (type == "Name")
					{
						if (iss >> str)
							placeholder_model->name = str;
					}
					else if (type == "Path")
					{
						std::string model_path;
						if (iss >> model_path)
							placeholder_model->model_path = model_path;
					}
					else if (type == "Textures")
					{
						unsigned int t1, t2, t3, t4;
						if (iss >> t1 >> t2 >> t3 >> t4)
							placeholder_model->textureTypeAmounts = { t1, t2, t3, t4 };
					}
				}

				if (type == "Entity")
				{
					on_entity = true;
					if (iss >> id)
						placeholder_entity->id = id;
				}
				if (type == "--E--")
				{
					on_entity = false;
					Entity* new_entity = new Entity(*placeholder_entity);

					if (collider || physics_body)
						physics_system->AddObject(new_entity);

					if (mesh_data)
					{
						MeshData* new_mesh_data = new MeshData(*mesh_data);
						new_entity->AddComponent<MeshData>(new_mesh_data);
						mesh_data = nullptr;
					}
					if (skinned_mesh)
					{
						SkinnedMesh* new_skinned_mesh = new SkinnedMesh(*skinned_mesh);
						new_entity->AddComponent<SkinnedMesh>(new_skinned_mesh);
						skinned_mesh = nullptr;
					}
					if (physics_body)
					{
						PhysicsBody* new_physics_body = new PhysicsBody(*physics_body);
						new_entity->AddComponent<PhysicsBody>(new_physics_body);
						physics_body = nullptr;
					}
					if (collider)
					{
						Collider* new_collider = new Collider(*collider);
						new_entity->AddComponent<Collider>(new_collider);
						collider = nullptr;
					}
					if (animator)
					{
						// add animator
					}
					if (dir_light)
					{
						DirectionalLight* new_light = new DirectionalLight(*dir_light);
						new_entity->AddComponent<DirectionalLight>(new_light);
						lighting_system->dirLight = new_light;
						dir_light = nullptr;
					}
					if (point_light)
					{
						PointLight* new_light = new PointLight(*point_light);
						new_entity->AddComponent<PointLight>(new_light);
						lighting_system->pointLights->push_back(new_light);
						point_light = nullptr;
					}
					if (spot_light)
					{
						SpotLight* new_light = new SpotLight(*spot_light);
						new_entity->AddComponent<SpotLight>(new_light);
						lighting_system->spotLights->push_back(new_light);
						spot_light = nullptr;
					}

					entities->push_back(new_entity);

					for (Shader* shader : *shaders)
					{
						if (shader->id == id)
							shader->AddEntity(new_entity);
					}

					placeholder_entity = new Entity();
				}

				if (on_entity)
				{
					if (type == "Component")
						on_component = true;
					if (type == "--C--")
						on_component = false;

					if (!on_component)
					{
						if (type == "Name")
						{
							if (iss >> str)
								placeholder_entity->name = str;
						}
						else if (type == "Position")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.Position = glm::vec3(x, y, z);
						}
						else if (type == "Scale")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.Scale = glm::vec3(x, y, z);
						}
						else if (type == "Rotation")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.EulerAngles = glm::vec3(x, y, z);
						}
						else if (type == "HasShadows")
						{
							if (iss >> enabled)
								placeholder_entity->has_shadows = enabled;
						}
						else if (type == "ShaderID")
						{
							if (iss >> id)
							{
								placeholder_entity->shader_id = id;
							}
						}
					}

					if (on_component)
					{
						if (type == "Type")
							iss >> c_type;

						if (c_type == MESH)
						{
							if (!mesh_data)
								mesh_data = new MeshData(NULL, NULL);

							if (type == "Enabled")
							{
								if (iss >> enabled)
									mesh_data->enabled = (bool)enabled;
							}
							else if (type == "ModelID")
							{
								unsigned int model_id = 0;
								if (iss >> model_id)
									mesh_data->model_id = model_id;

								for (unsigned int i = 0; i < (*models).size(); ++i)
								{
									if ((*models)[i]->id == mesh_data->model_id)
										mesh_data->model = (*models)[i];
								}
								if (mesh_data->model == nullptr)
									Log::Error("Error finding " + std::to_string(mesh_data->model_id) + " model id");
							}
						}
						else if (c_type == SKINNED_MESH)
						{
							/*if (!skinned_mesh)
								skinned_mesh = new SkinnedMesh(NULL, NULL);

							if (type == "Enabled")
							{
								if (iss >> enabled)
									skinned_mesh->enabled = (bool)enabled;
							}
							else if (type == "ModelID")
							{
								unsigned int model_id = 0;
								if (iss >> model_id)
									skinned_mesh->model_id = model_id;

								for (unsigned int i = 0; i < (*models).size(); ++i)
								{
									if ((*models)[i]->id == skinned_mesh->model_id)
										skinned_mesh->model = (*models)[i];
								}
								if (skinned_mesh->model == nullptr)
									Log::Error("Error finding " + std::to_string(skinned_mesh->model_id) + " model id");
							}*/
						}
						else if (c_type == PHYSICS)
						{
							if (!physics_body)
								physics_body = new PhysicsBody();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									physics_body->enabled = (bool)enabled;
							}
							else if (type == "Velocity")
							{
								if (iss >> x >> y >> z)
									physics_body->Velocity = glm::vec3(x, y, z);
							}
							else if (type == "Force")
							{
								if (iss >> x >> y >> z)
									physics_body->Force = glm::vec3(x, y, z);
							}
							else if (type == "Mass")
							{
								float mass;
								if (iss >> mass)
									physics_body->Mass = mass;
							}
						}
						else if (c_type == COLLIDER)
						{
							if (!collider)
								collider = new Collider();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									collider->enabled = (bool)enabled;
							}
							else if (type == "Position")
							{
								if (iss >> x >> y >> z)
									collider->Position = glm::vec3(x, y, z);
							}
							else if (type == "Scale")
							{
								if (iss >> x >> y >> z)
									collider->Scale = glm::vec3(x, y, z);
							}
						}
						else if (c_type == ANIMATOR)
						{

						}
						else if (c_type == AUDIO)
						{

						}
						// -- LIGHTING COMPONENTS
						else if (c_type == DIR_LIGHT)
						{
							if (!dir_light && !lighting_system->dirLight)
								dir_light = new DirectionalLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									dir_light->enabled = (bool)enabled;
							}
							else if (type == "Direction")
							{
								if (iss >> x >> y >> z)
									dir_light->Position = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									dir_light->Color = glm::vec3(x, y, z);
							}
						}
						else if (c_type == PNT_LIGHT)
						{
							if (!point_light)
								point_light = new PointLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									point_light->enabled = (bool)enabled;
							}
							else if (type == "Range")
							{
								if (iss >> x >> y >> z)
									point_light->Scale = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									point_light->Color = glm::vec3(x, y, z);
							}
						}
						else if (c_type == SPT_LIGHT)
						{
							if (!spot_light)
								spot_light = new SpotLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									spot_light->enabled = (bool)enabled;
							}
							else if (type == "Direction")
							{
								if (iss >> x >> y >> z)
									spot_light->Direction = glm::vec3(x, y, z);
							}
							else if (type == "Range")
							{
								if (iss >> x >> y >> z)
									spot_light->Scale = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									spot_light->Color = glm::vec3(x, y, z);
							}
						}
						// --
					}
				}
			}
		}
		delete placeholder_entity;
		delete placeholder_model;
		delete placeholder_shader;
		delete mesh_data;
		delete skinned_mesh;
		delete physics_body;
		delete collider;
		delete animator;
		delete dir_light;
		delete point_light;
		delete spot_light;

		file.close();

		return true;
	}

	bool Scene::LoadWithoutShaders(std::vector<Entity*>*& entities, std::vector<Model*>*& models, PhysicsSystem*& physics_system, LightingSystem*& lighting_system)
	{
		models = new std::vector<Model*>;
		entities = new std::vector<Entity*>;
		physics_system = new PhysicsSystem;
		lighting_system = new LightingSystem;

		std::string& path = GetPath();

		std::ifstream file(path);
		std::string line;

		bool on_model = false;
		bool on_entity = false;
		bool on_component = false;
		bool on_shader = false;
		float x, y, z;
		unsigned int id;
		unsigned int c_type = NONE;
		unsigned int enabled;
		std::string str;

		Entity* placeholder_entity = new Entity();
		MeshData* mesh_data = nullptr;
		PhysicsBody* physics_body = nullptr;
		Collider* collider = nullptr;
		AnimatorData* animator = nullptr;
		DirectionalLight* dir_light = nullptr;
		PointLight* point_light = nullptr;
		SpotLight* spot_light = nullptr;

		Model* placeholder_model = new Model();

		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::string type;

			if (iss >> type)
			{
				if (type == "Model")
				{
					on_model = true;
					if (iss >> id)
						placeholder_model->id = id;
				}
				if (type == "--M--")
				{
					on_model = false;
					Model* new_model = new Model(*placeholder_model);
					new_model->LoadModel(new_model->model_path, new_model->textureTypeAmounts);
					models->push_back(new_model);
					placeholder_model = new Model();
				}

				if (on_model)
				{
					if (type == "Name")
					{
						if (iss >> str)
							placeholder_model->name = str;
					}
					else if (type == "Path")
					{
						std::string model_path;
						if (iss >> model_path)
							placeholder_model->model_path = model_path;
					}
					else if (type == "Textures")
					{
						unsigned int t1, t2, t3, t4;
						if (iss >> t1 >> t2 >> t3 >> t4)
							placeholder_model->textureTypeAmounts = { t1, t2, t3, t4 };
					}
				}

				if (type == "Entity")
				{
					on_entity = true;
					if (iss >> id)
						placeholder_entity->id = id;
				}
				if (type == "--E--")
				{
					on_entity = false;
					Entity* new_entity = new Entity(*placeholder_entity);

					if (collider || physics_body)
						physics_system->AddObject(new_entity);

					if (mesh_data)
					{
						MeshData* new_mesh_data = new MeshData(*mesh_data);
						new_entity->AddComponent<MeshData>(new_mesh_data);
						mesh_data = nullptr;
					}
					if (physics_body)
					{
						PhysicsBody* new_physics_body = new PhysicsBody(*physics_body);
						new_entity->AddComponent<PhysicsBody>(new_physics_body);
						physics_body = nullptr;
					}
					if (collider)
					{
						Collider* new_collider = new Collider(*collider);
						new_entity->AddComponent<Collider>(new_collider);
						collider = nullptr;
					}
					if (animator)
					{
						// add animator
					}
					if (dir_light)
					{
						DirectionalLight* new_light = new DirectionalLight(*dir_light);
						new_entity->AddComponent<DirectionalLight>(new_light);
						lighting_system->dirLight = new_light;
						dir_light = nullptr;
					}
					else if (point_light)
					{
						PointLight* new_light = new PointLight(*point_light);
						new_entity->AddComponent<PointLight>(new_light);
						lighting_system->pointLights->push_back(new_light);
						point_light = nullptr;
					}
					else if (spot_light)
					{
						SpotLight* new_light = new SpotLight(*spot_light);
						new_entity->AddComponent<SpotLight>(new_light);
						lighting_system->spotLights->push_back(new_light);
						spot_light = nullptr;
					}

					entities->push_back(new_entity);
					placeholder_entity = new Entity();
				}

				if (on_entity)
				{
					if (type == "Component")
						on_component = true;
					if (type == "--C--")
						on_component = false;

					if (!on_component)
					{
						if (type == "Name")
						{
							if (iss >> str)
								placeholder_entity->name = str;
						}
						else if (type == "Position")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.Position = glm::vec3(x, y, z);
						}
						else if (type == "Scale")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.Scale = glm::vec3(x, y, z);
						}
						else if (type == "Rotation")
						{
							if (iss >> x >> y >> z)
								placeholder_entity->transform.EulerAngles = glm::vec3(x, y, z);
						}
						else if (type == "HasShadows")
						{
							if (iss >> enabled)
								placeholder_entity->has_shadows = enabled;
						}
						else if (type == "ShaderID")
						{
							if (iss >> id)
								placeholder_entity->shader_id = id;
						}
					}

					if (on_component)
					{
						if (type == "Type")
							iss >> c_type;

						if (c_type == MESH)
						{
							if (!mesh_data)
								mesh_data = new MeshData(NULL, NULL);

							if (type == "Enabled")
							{
								if (iss >> enabled)
									mesh_data->enabled = (bool)enabled;
							}
							else if (type == "ModelID")
							{
								unsigned int model_id = 0;
								if (iss >> model_id)
									mesh_data->model_id = model_id;

								for (unsigned int i = 0; i < (*models).size(); ++i)
								{
									if ((*models)[i]->id == mesh_data->model_id)
										mesh_data->model = (*models)[i];
								}
								if (mesh_data->model == nullptr)
									Log::Error("Error finding " + std::to_string(mesh_data->model_id) + " model id");
							}
						}
						else if (c_type == PHYSICS)
						{
							if (!physics_body)
								physics_body = new PhysicsBody();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									physics_body->enabled = (bool)enabled;
							}
							else if (type == "Velocity")
							{
								if (iss >> x >> y >> z)
									physics_body->Velocity = glm::vec3(x, y, z);
							}
							else if (type == "Force")
							{
								if (iss >> x >> y >> z)
									physics_body->Force = glm::vec3(x, y, z);
							}
							else if (type == "Mass")
							{
								float mass;
								if (iss >> mass)
									physics_body->Mass = mass;
							}
						}
						else if (c_type == COLLIDER)
						{
							if (!collider)
								collider = new Collider();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									collider->enabled = (bool)enabled;
							}
							else if (type == "Position")
							{
								if (iss >> x >> y >> z)
									collider->Position = glm::vec3(x, y, z);
							}
							else if (type == "Scale")
							{
								if (iss >> x >> y >> z)
									collider->Scale = glm::vec3(x, y, z);
							}
						}
						else if (c_type == ANIMATOR)
						{

						}
						else if (c_type == AUDIO)
						{

						}
						// -- LIGHTING COMPONENTS
						else if (c_type == DIR_LIGHT)
						{
							if (!dir_light)
								dir_light = new DirectionalLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									dir_light->enabled = (bool)enabled;
							}
							else if (type == "Direction")
							{
								if (iss >> x >> y >> z)
									dir_light->Position = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									dir_light->Color = glm::vec3(x, y, z);
							}
						}
						else if (c_type == PNT_LIGHT)
						{
							if (!point_light)
								point_light = new PointLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									point_light->enabled = (bool)enabled;
							}
							else if (type == "Range")
							{
								if (iss >> x >> y >> z)
									point_light->Scale = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									point_light->Color = glm::vec3(x, y, z);
							}
						}
						else if (c_type == SPT_LIGHT)
						{
							if (!spot_light)
								spot_light = new SpotLight();

							if (type == "Enabled")
							{
								if (iss >> enabled)
									spot_light->enabled = (bool)enabled;
							}
							else if (type == "Direction")
							{
								if (iss >> x >> y >> z)
									spot_light->Direction = glm::vec3(x, y, z);
							}
							else if (type == "Range")
							{
								if (iss >> x >> y >> z)
									spot_light->Scale = glm::vec3(x, y, z);
							}
							else if (type == "Color")
							{
								if (iss >> x >> y >> z)
									spot_light->Color = glm::vec3(x, y, z);
							}
						}
						// --
					}
				}
			}
		}
		delete placeholder_entity;
		delete placeholder_model;
		delete mesh_data;
		delete physics_body;
		delete collider;
		delete animator;
		delete dir_light;
		delete point_light;
		delete spot_light;

		file.close();

		return true;
	}

	bool Scene::Save(std::vector<Entity*>* entities, std::vector<Model*>* models, std::vector<Shader*>*& shaders)
	{
		Log::Info("Saving...");

		std::vector<Model*>& modelsRef = *models;
		std::vector<Entity*>& entitiesRef = *entities;
		std::vector<Shader*>& shadersRef = *shaders;

		std::string& path = GetPath();
		std::string filecontents;
		std::string shadercontents;
		std::string modelcontents;
		std::string entitycontents;

		std::string data;

		filecontents += "Scene: " + GetName();

		for (int i = 0; i < shadersRef.size(); ++i)
		{
			data = shadersRef[i]->Save();
			shadercontents += data;
		}

		for (int i = 0; i < modelsRef.size(); ++i)
		{
			data = modelsRef[i]->Save();
			modelcontents += data;
		}

		for (int i = 0; i < entitiesRef.size(); ++i)
		{
			data = entitiesRef[i]->Save();
			entitycontents += data;
		}

		filecontents +=
			"\n  Shaders: " + shadercontents + 
			"\n  Models: " + modelcontents +
			"\n  Entities: " + entitycontents;

		std::ofstream wfile(path);

		wfile << filecontents;

		wfile.close();
		Log::Info("Saved");

		return true;
	}
}