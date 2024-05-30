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

	bool Scene::Load(std::vector<Entity*>*& entities, std::vector<Model*>*& models, PhysicsSystem*& physics_system)
	{
		models = new std::vector<Model*>;
		entities = new std::vector<Entity*>;
		physics_system = new PhysicsSystem;

		std::string& path = GetPath();

		std::ifstream file(path);
		std::string line;

		bool on_model = false;
		bool on_entity = false;
		bool on_component = false;
		float x, y, z;
		unsigned int id;
		unsigned int c_type = NONE;
		unsigned int enabled;
		std::string name;

		Entity* placeholder_entity = new Entity();
		MeshData* mesh_data = nullptr;
		PhysicsBody* physics_body = nullptr;
		Collider* collider = nullptr;
		Animator* animator = nullptr;

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
						if (iss >> name)
							placeholder_model->name = name;
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
							if (iss >> name)
								placeholder_entity->name = name;
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

		file.close();

		return true;
	}

	bool Scene::Save(std::vector<Entity*>* entities, std::vector<Model*>* models)
	{
		Log::Info("Saving...");

		std::vector<Model*>& modelsRef = *models;
		std::vector<Entity*>& entitiesRef = *entities;

		std::string& path = GetPath();
		std::string filecontents;
		std::string modelcontents;
		std::string entitycontents;

		std::string data;

		filecontents += "Scene: " + GetName();

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
			"\n  Models: " + modelcontents +
			"\n  Entities: " + entitycontents;

		std::ofstream wfile(path);

		wfile << filecontents;

		wfile.close();
		Log::Info("Saved");

		return true;
	}
}