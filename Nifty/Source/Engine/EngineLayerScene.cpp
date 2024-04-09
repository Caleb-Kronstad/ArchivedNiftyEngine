#include "nfpch.h"
#include "EngineLayer.h"

namespace Nifty
{
	void EngineLayer::RenderScene(Shader& shader, const bool& checkCollision)
	{
		std::vector<Shader*>& shadersRef = *m_Shaders;
		std::vector<Model*>& modelsRef = *m_Models;
		std::vector<GameObject*>& objectsRef = *m_GameObjects;

		for (unsigned int i = 0; i < objectsRef.size(); ++i)
		{
			// CHECK COLLISION BETWEEN OBJECTS
			if (checkCollision && objectsRef[i]->colliderActive)
			{
				if (!objectsRef[i]->colliding)
				{
					for (unsigned int j = 0; j < objectsRef.size(); ++j)
					{
						GameObject& collisionObject = *objectsRef[j];
						if (objectsRef[j] != objectsRef[i] && objectsRef[i]->CheckCollision(collisionObject))
						{
							objectsRef[i]->colliding = true;
							objectsRef[i]->currentCollidingObject = objectsRef[j];
						}
					}
				}
				else
				{
					GameObject& collisionObject = *objectsRef[i]->currentCollidingObject;
					if (!objectsRef[i]->CheckCollision(collisionObject))
					{
						objectsRef[i]->colliding = false;
						objectsRef[i]->currentCollidingObject = nullptr;
					}
				}
			}
			objectsRef[i]->Draw(shader, *m_Matrix);
		}
	}

	bool EngineLayer::LoadScene(Scene* scene)
	{
		m_Models = new std::vector<Model*>;
		m_GameObjects = new std::vector<GameObject*>;

		std::string& path = scene->GetPath();

		std::ifstream file(path);
		std::string line;

		// LOAD MODELS
		while (std::getline(file, line))
		{
			std::size_t entitycheck = line.find("entity");
			std::size_t typecheck = line.find("Model");
			if (entitycheck != std::string::npos && typecheck != std::string::npos)
			{
				std::size_t pathpos = line.find("path");
				std::size_t pathend = line.find("path_end");
				std::size_t idpos = line.find("id");
				std::size_t idend = line.find("id_end");
				std::size_t namepos = line.find("name");
				std::size_t nameend = line.find("name_end");
				std::size_t texturespos = line.find("textures");
				std::size_t texturesend = line.find("textures_end");

				std::string modelpath = line.substr(pathpos + 5, pathend - pathpos - 6);
				std::string name = line.substr(namepos + 5, nameend - namepos - 6);
				unsigned int modelid = std::stoi(line.substr(idpos + 3, idend - idpos - 3));
				unsigned int tex1 = std::stoi(line.substr(texturespos + 9, texturesend - texturespos - 15));
				unsigned int tex2 = std::stoi(line.substr(texturespos + 11, texturesend - texturespos - 13));
				unsigned int tex3 = std::stoi(line.substr(texturespos + 13, texturesend - texturespos - 11));
				unsigned int tex4 = std::stoi(line.substr(texturespos + 15, texturesend - texturespos - 9));

				Model* model = new Model(modelpath, { tex1, tex2, tex3, tex4 }, modelid);
				model->name = name;
				m_Models->push_back(model);
			}
		}

		file.close();

		file = std::ifstream(path);
		line = std::string();

		// LOAD GAME OBJECTS
		while (std::getline(file, line))
		{
			std::size_t entitycheck = line.find("entity");
			std::size_t typecheck = line.find("GameObject");
			if (entitycheck != std::string::npos && typecheck != std::string::npos)
			{
				// ITS SO MUCH I PROBABLY HAVE TO MAKE THIS BETTER LATER BUT FUCK IT
				std::size_t idpos = line.find("id");
				std::size_t idend = line.find("id_end");
				std::size_t modelidpos = line.find("model_id");
				std::size_t modelidend = line.find("model_id_end");
				std::size_t namepos = line.find("name");
				std::size_t nameend = line.find("name_end");

				std::size_t pospos = line.find("pos");
				std::size_t posend = line.find("pos_end");
				std::size_t scalepos = line.find("scale");
				std::size_t scaleend = line.find("scale_end");
				std::size_t euleranglespos = line.find("euler_angles");
				std::size_t euleranglesend = line.find("euler_angles_end");
				std::size_t anglepos = line.find("rotation_angle");
				std::size_t angleend = line.find("rotation_angle_end");

				std::size_t physicsflagpos = line.find("physics_flag");
				std::size_t physicsflagend = line.find("physics_flag_end");
				std::size_t masspos = line.find("mass");
				std::size_t massend = line.find("mass_end");
				std::size_t velocitypos = line.find("velocity");
				std::size_t velocityend = line.find("velocity_end");
				std::size_t forcepos = line.find("force");
				std::size_t forceend = line.find("force_end");

				std::size_t colliderflagpos = line.find("collider_flag");
				std::size_t colliderflagend = line.find("collider_flag_end");

				unsigned int objectid = std::stoi(line.substr(idpos + 3, idend - idpos - 3));
				unsigned int modelid = std::stoi(line.substr(modelidpos + 9, modelidend - modelidpos - 9));
				std::string name = line.substr(namepos + 5, nameend - namepos - 6);
				bool colliderflag = std::stoi(line.substr(colliderflagpos + 14, colliderflagend - colliderflagpos - 14));

				float angle = std::stof(line.substr(anglepos + 15, angleend - anglepos - 15));
				bool physicsflag = std::stoi(line.substr(physicsflagpos + 13, physicsflagend - physicsflagpos - 13));
				float mass = std::stof(line.substr(masspos + 5, massend - masspos - 5));

				std::string posstr = line.substr(pospos + 4, posend - pospos - 4);
				glm::vec3 pos = glm::vec3(
					std::stof(posstr.substr(posstr.find("x") + 1, posstr.find("y") - 1)),
					std::stof(posstr.substr(posstr.find("y") + 1, posstr.find("z") - 1)),
					std::stof(posstr.substr(posstr.find("z") + 1)));

				std::string scalestr = line.substr(scalepos + 6, scaleend - scalepos - 6);
				glm::vec3 scale = glm::vec3(
					std::stof(scalestr.substr(scalestr.find("x") + 1, scalestr.find("y") - 1)),
					std::stof(scalestr.substr(scalestr.find("y") + 1, scalestr.find("z") - 1)),
					std::stof(scalestr.substr(scalestr.find("z") + 1)));

				std::string euleranglesstr = line.substr(euleranglespos + 12, euleranglesend - euleranglespos - 12);
				glm::vec3 eulerangles = glm::vec3(
					std::stof(euleranglesstr.substr(euleranglesstr.find("x") + 1, euleranglesstr.find("y") - 1)),
					std::stof(euleranglesstr.substr(euleranglesstr.find("y") + 1, euleranglesstr.find("z") - 1)),
					std::stof(euleranglesstr.substr(euleranglesstr.find("z") + 1)));

				std::string velocitystr = line.substr(velocitypos + 9, velocityend - velocitypos - 9);
				glm::vec3 velocity = glm::vec3(
					std::stof(velocitystr.substr(velocitystr.find("x") + 1, velocitystr.find("y") - 1)),
					std::stof(velocitystr.substr(velocitystr.find("y") + 1, velocitystr.find("z") - 1)),
					std::stof(velocitystr.substr(velocitystr.find("z") + 1)));

				std::string forcestr = line.substr(forcepos + 9, forceend - forcepos - 9);
				glm::vec3 force = glm::vec3(
					std::stof(forcestr.substr(forcestr.find("x") + 1, forcestr.find("y") - 1)),
					std::stof(forcestr.substr(forcestr.find("y") + 1, forcestr.find("z") - 1)),
					std::stof(forcestr.substr(forcestr.find("z") + 1)));

				Model* model = nullptr;
				for (unsigned int i = 0; i < (*m_Models).size(); ++i)
				{
					if ((*m_Models)[i]->id == modelid)
					{
						model = (*m_Models)[i];
					}
				}
				if (model == nullptr)
				{
					Log::Error("Error finding model that corresponds to - " + name + " - object");
					return false;
				}

				GameObject* gameObject = new GameObject(
					Transform(pos, scale, eulerangles, angle, physicsflag, velocity, force, mass),
					model, name, objectid, modelid);
				gameObject->colliderActive = colliderflag;
				m_GameObjects->push_back(gameObject);
			}
		}

		file.close();

		return true;
	}

	bool EngineLayer::SaveScene(Scene* scene)
	{
		Log::Info("Saving...");

		std::vector<Shader*>& shadersRef = *m_Shaders;
		std::vector<Model*>& modelsRef = *m_Models;
		std::vector<GameObject*>& objectsRef = *m_GameObjects;

		std::string& path = scene->GetPath();
		std::ifstream rfile(path);
		std::string line;
		std::string filecontents;
		std::string modelcontents;
		std::string objectcontents;

		filecontents += "Scene: " + scene->GetName() + "\n";

		while (std::getline(rfile, line))
		{
			std::size_t entitycheck = line.find("entity");
			if (entitycheck != std::string::npos)
			{
				std::size_t modelcheck = line.find("Model");
				if (modelcheck != std::string::npos)
				{
					for (unsigned int i = 0; i < modelsRef.size(); ++i)
					{
						std::size_t idcheck = line.find("id " + std::to_string(modelsRef[i]->id));
						if (idcheck != std::string::npos)
						{
							std::string data =
								"entity " + modelsRef[i]->type + " entity_end" +
								" : id " + std::to_string(modelsRef[i]->id) + " id_end" +
								" : name " + modelsRef[i]->name + " name_end" +
								" : path " + modelsRef[i]->model_path + " path_end" +
								" : textures " + std::to_string(modelsRef[i]->textureTypeAmounts[0]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[1]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[2]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[3]) + " textures_end";
							line.replace(line.begin(), line.end(), data);

							modelsRef[i]->saved = true;

							modelcontents += line + "\n";
						}
					}
				}

				std::size_t objectcheck = line.find("GameObject");
				if (objectcheck != std::string::npos)
				{
					for (unsigned int i = 0; i < objectsRef.size(); ++i)
					{
						std::size_t idcheck = line.find("id " + std::to_string(objectsRef[i]->id));
						if (idcheck != std::string::npos)
						{
							std::string data =
								"entity " + objectsRef[i]->type + " entity_end" +
								" : id " + std::to_string(objectsRef[i]->id) + " id_end" +
								" : model_id " + std::to_string(objectsRef[i]->modelid) + " model_id_end" +
								" : name " + objectsRef[i]->name + " name_end" +
								" : pos x" + std::to_string(objectsRef[i]->transform.Position.x) + " y" + std::to_string(objectsRef[i]->transform.Position.y) + " z" + std::to_string(objectsRef[i]->transform.Position.z) + " pos_end" +
								" : scale x" + std::to_string(objectsRef[i]->transform.Scale.x) + " y" + std::to_string(objectsRef[i]->transform.Scale.y) + " z" + std::to_string(objectsRef[i]->transform.Scale.z) + " scale_end" +
								" : euler_angles x" + std::to_string(objectsRef[i]->transform.EulerAngles.x) + " y" + std::to_string(objectsRef[i]->transform.EulerAngles.y) + " z" + std::to_string(objectsRef[i]->transform.EulerAngles.z) + " euler_angles_end" +
								" : rotation_angle " + std::to_string(objectsRef[i]->transform.Angle) + " rotation_angle_end" +
								" : physics_flag " + std::to_string(objectsRef[i]->transform.PhysicsActive) + " physics_flag_end" +
								" : mass " + std::to_string(objectsRef[i]->transform.Mass) + " mass_end" +
								" : velocity x" + std::to_string(objectsRef[i]->transform.Velocity.x) + " y" + std::to_string(objectsRef[i]->transform.Velocity.y) + " z" + std::to_string(objectsRef[i]->transform.Velocity.z) + " velocity_end" +
								" : force x" + std::to_string(objectsRef[i]->transform.Force.x) + " y" + std::to_string(objectsRef[i]->transform.Force.y) + " z" + std::to_string(objectsRef[i]->transform.Force.z) + " force_end" +
								" : collider_flag " + std::to_string(objectsRef[i]->colliderActive) + " collider_flag_end";
							line.replace(line.begin(), line.end(), data);

							objectsRef[i]->saved = true;

							objectcontents += line + "\n";
						}
					}
				}
			}
		}
		rfile.close();

		for (int i = 0; i < objectsRef.size(); ++i)
		{
			if (objectsRef[i]->saved == false)
			{
				std::string data =
					"entity " + objectsRef[i]->type + " entity_end" +
					" : id " + std::to_string(objectsRef[i]->id) + " id_end" +
					" : model_id " + std::to_string(objectsRef[i]->modelid) + " model_id_end" +
					" : name " + objectsRef[i]->name + " name_end" +
					" : pos x" + std::to_string(objectsRef[i]->transform.Position.x) + " y" + std::to_string(objectsRef[i]->transform.Position.y) + " z" + std::to_string(objectsRef[i]->transform.Position.z) + " pos_end" +
					" : scale x" + std::to_string(objectsRef[i]->transform.Scale.x) + " y" + std::to_string(objectsRef[i]->transform.Scale.y) + " z" + std::to_string(objectsRef[i]->transform.Scale.z) + " scale_end" +
					" : euler_angles x" + std::to_string(objectsRef[i]->transform.EulerAngles.x) + " y" + std::to_string(objectsRef[i]->transform.EulerAngles.y) + " z" + std::to_string(objectsRef[i]->transform.EulerAngles.z) + " euler_angles_end" +
					" : rotation_angle " + std::to_string(objectsRef[i]->transform.Angle) + " rotation_angle_end" +
					" : physics_flag " + std::to_string(objectsRef[i]->transform.PhysicsActive) + " physics_flag_end" +
					" : mass " + std::to_string(objectsRef[i]->transform.Mass) + " mass_end" +
					" : velocity x" + std::to_string(objectsRef[i]->transform.Velocity.x) + " y" + std::to_string(objectsRef[i]->transform.Velocity.y) + " z" + std::to_string(objectsRef[i]->transform.Velocity.z) + " velocity_end" +
					" : force x" + std::to_string(objectsRef[i]->transform.Force.x) + " y" + std::to_string(objectsRef[i]->transform.Force.y) + " z" + std::to_string(objectsRef[i]->transform.Force.z) + " force_end"+
					" : collider_flag " + std::to_string(objectsRef[i]->colliderActive) + " collider_flag_end";

				objectsRef[i]->saved = true;

				objectcontents += data + "\n";
			}
		}

		for (int i = 0; i < modelsRef.size(); ++i)
		{
			if (modelsRef[i]->saved == false)
			{
				std::string data =
					"entity " + modelsRef[i]->type + " entity_end" +
					" : id " + std::to_string(modelsRef[i]->id) + " id_end" +
					" : name " + modelsRef[i]->name + " name_end" +
					" : path " + modelsRef[i]->model_path + " path_end" +
					" : textures " + std::to_string(modelsRef[i]->textureTypeAmounts[0]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[1]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[2]) + " " + std::to_string(modelsRef[i]->textureTypeAmounts[3]) + " textures_end";

				modelsRef[i]->saved = true;

				modelcontents += data + "\n";
			}
		}

		filecontents += modelcontents + objectcontents;

		std::ofstream wfile(path);

		wfile << filecontents;

		wfile.close();
		Log::Info("Saved");

		return true;
	}
}