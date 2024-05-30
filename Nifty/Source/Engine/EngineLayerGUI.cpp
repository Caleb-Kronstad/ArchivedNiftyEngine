#include "nfpch.h"
#include "EngineLayer.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace Nifty
{
	void EngineLayer::OnImGuiRender()
	{
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		Window& viewport = app.GetViewport();

		std::vector<Shader*>& shadersRef = *m_Shaders;
		std::vector<Model*>& modelsRef = *m_Models;
		std::vector<Entity*>& entitiesRef = *m_Entities;

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.DisplaySize = ImVec2(window.GetWidth(), window.GetHeight());

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		ImGuiWindowFlags menu_flags = window_flags;
		menu_flags |= ImGuiWindowFlags_NoScrollbar;
		menu_flags |= ImGuiWindowFlags_NoTitleBar;
		menu_flags |= ImGuiWindowFlags_NoBackground;
		menu_flags |= ImGuiWindowFlags_MenuBar;

		ImGuiWindowFlags popup_flags = 0;
		popup_flags |= ImGuiWindowFlags_NoResize;
		popup_flags |= ImGuiWindowFlags_NoCollapse;

		ImGuiWindowFlags viewport_flags = 0;
		viewport_flags |= ImGuiWindowFlags_NoScrollbar;
		viewport_flags |= ImGuiWindowFlags_NoTitleBar;
		viewport_flags |= ImGuiWindowFlags_NoBackground;
		viewport_flags |= ImGuiWindowFlags_MenuBar;
		viewport_flags |= ImGuiWindowFlags_NoResize;
		viewport_flags |= ImGuiWindowFlags_NoCollapse;
		viewport_flags |= ImGuiWindowFlags_NoMove;

		if (!m_GUIFontLoaded)
		{
			ImGui::PushFont(m_GUIFont);
			m_GUIFontLoaded = true;
		}

		// -----MENU BAR-----------------
		ImGui::SetNextWindowSize(ImVec2(window.GetWidth(), window.GetHeight() / 20), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::Begin("Menu", (bool*)false, menu_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (app.GetGameRunState())
				{
					if (ImGui::MenuItem("Stop Running"))
					{
						m_Scene->Load(m_Entities, m_Models, m_PhysicsSystem);
						app.GetGameRunState() = false;
					}
				}
				else
				{
					if (ImGui::MenuItem("Run Scene"))
					{
						app.GetGameRunState() = true;
					}
				}

				if (ImGui::MenuItem("Save Scene"))
				{
					if (app.GetGameRunState() == false)
						m_Scene->Save(m_Entities, m_Models);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Add Entity", NULL, &m_AddEntityWindowOpen))
					m_AddModelWindowOpen = false;
				if (ImGui::MenuItem("Add Model", NULL, &m_AddModelWindowOpen))
					m_AddEntityWindowOpen = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
		// ------------------------------


		// -----FILE MANAGER-------------
		ImGui::SetNextWindowSize(ImVec2(viewport.GetWidth(), window.GetHeight() - viewport.GetHeight()), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(window.GetWidth() - viewport.GetWidth(), viewport.GetHeight()), ImGuiCond_Always);
		ImGui::Begin("File Manager", (bool*)false, window_flags);

		for (unsigned int i = 0; i < modelsRef.size(); ++i)
		{
			ImGui::BeginGroup();

			ImGui::Image((void*)m_FileImageTexture, ImVec2(100, 100));
			if (ImGui::Button(modelsRef[i]->name.c_str(), ImVec2(100, 15))) // checks if button is pressed (and if it's allowed to be pressed)
			{
				m_PropertiesObject.id = modelsRef[i]->id;
				m_PropertiesObject.modelptr = modelsRef[i];
			}

			ImGui::EndGroup();
			ImGui::SameLine();
		}

		ImGui::End();
		// ------------------------------


		// -----EXPLORER-----------------
		ImGui::SetNextWindowSize(ImVec2(window.GetWidth() - viewport.GetWidth(), window.GetHeight() / 2 - (window.GetHeight() / 50)), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, window.GetHeight() / 50), ImGuiCond_Always);
		ImGui::Begin("Explorer", (bool*)false, window_flags);

		for (unsigned int i = 0; i < entitiesRef.size(); ++i)
		{
			if (ImGui::Button(entitiesRef[i]->name.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15))) // checks if button is pressed (and if it's allowed to be pressed)
			{
				m_PropertiesObject.id = entitiesRef[i]->id;
				m_PropertiesObject.entityptr = entitiesRef[i];
			}
					
		}

		ImGui::End();
		// ------------------------------


		// -----PROPERTIES---------------
		ImGui::SetNextWindowSize(ImVec2(window.GetWidth() - viewport.GetWidth(), window.GetHeight() / 2), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, window.GetHeight() / 2), ImGuiCond_Always);
		ImGui::Begin("Properties", (bool*)false, window_flags);

		if (m_PropertiesObject.id != 0)
		{
			ImGui::BeginGroup();
			// MODELS PROPERTY PAGE
			if (m_PropertiesObject.id > 30000000 && m_PropertiesObject.id < 40000000)
			{
				Model* currentModel = m_PropertiesObject.modelptr;
				if (currentModel != nullptr)
				{
					std::string name = currentModel->name + " (" + std::to_string(currentModel->id) + ")";
					std::string pathName = "Path";

					ImGui::Text(name.c_str());

					if (ImGui::RadioButton("OBJ", m_LoadModelType == MODEL_TYPE::OBJ))
						m_LoadModelType = MODEL_TYPE::OBJ;
					ImGui::SameLine();
					if (ImGui::RadioButton("DAE", m_LoadModelType == MODEL_TYPE::DAE))
						m_LoadModelType = MODEL_TYPE::DAE;

					if (ImGui::Button("Change File", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + m_ProjectPath;
						m_FilenameModel = std::string(MAX_PATH, '\0');

						OPENFILENAMEA ofn;
						ZeroMemory(&ofn, sizeof(OPENFILENAME));
						ofn.lStructSize = sizeof(OPENFILENAME);
						ofn.lpstrFile = (LPSTR)m_FilenameModel.c_str();
						ofn.nMaxFile = m_FilenameModel.size();
						ofn.lpstrInitialDir = m_MyDocuments.c_str();
						ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

						if (m_LoadModelType == MODEL_TYPE::OBJ)
						{
							ofn.lpstrFilter = "Obj Files\0*.obj\0Any file\0*.obj*\0";
							ofn.lpstrTitle = "Select .obj file";
						}
						else if (m_LoadModelType == MODEL_TYPE::DAE)
						{
							ofn.lpstrFilter = "Dae Files\0*.dae\0Any file\0*.dae*\0";
							ofn.lpstrTitle = "Select .dae file";
						}

						if (GetOpenFileNameA(&ofn))
						{
							m_FilenameModel.resize(m_FilenameModel.find('\0'));

							currentModel->model_path = m_FilenameModel;
							Log::Info("You chose the file: " + m_FilenameModel);
						}
						else
							Log::Info("You chose to cancel the file operation");
					}
					ImGui::SameLine();
					ImGui::Text(currentModel->model_path.c_str());

					ImGui::Spacing();
					ImGui::Text("Textures");
					// Display model textures
					for (unsigned int i = 0; i < currentModel->textures_loaded.size(); ++i)
					{
						ImGui::Image((void*)currentModel->textures_loaded[i].ID, ImVec2(100, 100));
						ImGui::SameLine();
						ImGui::Text(currentModel->textures_loaded[i].Type.c_str());
					}

					ImGui::Spacing();
					if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						// LATER ADD A CONFIRM DELETE MENU CAUSE THIS CAN CAUSE ISSUES
						if (modelsRef.size() > 1)
						{
							auto itr = std::find(modelsRef.begin(), modelsRef.end(), currentModel); // find in vector
							if (itr == modelsRef.end()) return; // check if model is in vector
							modelsRef.erase(itr); // remove from vector
							m_PropertiesObject.modelptr = nullptr;
							m_PropertiesObject.entityptr = nullptr;
							m_PropertiesObject.id = 0;
						}
						else
							Log::Error("Need at least 1 model in the scene");
					}
				}
			}
			// ENTITY PROPERTY PAGE
			if (m_PropertiesObject.id > 20000000 && m_PropertiesObject.id < 30000000)
			{
				Entity* currentEntity = m_PropertiesObject.entityptr;
				if (currentEntity != nullptr)
				{
					std::string name = currentEntity->name + " (" + std::to_string(currentEntity->id) + ")";

					ImGui::Text(name.c_str());

					ImGui::DragFloat("DragStep", &m_DragStep, 1.0f, 0.0f, 100.0f);

					// TRANSFORM COMPONENT
					if (glfwGetKey(window.GetNativeWindow(), Key::E) == GLFW_PRESS)
						m_GizmoType = ImGuizmo::TRANSLATE;
					if (glfwGetKey(window.GetNativeWindow(), Key::R) == GLFW_PRESS)
						m_GizmoType = ImGuizmo::ROTATE;
					if (glfwGetKey(window.GetNativeWindow(), Key::T) == GLFW_PRESS)
						m_GizmoType = ImGuizmo::SCALE;
					if (ImGui::CollapsingHeader("Transform"))
					{
						if (ImGui::RadioButton("Translate", m_GizmoType == ImGuizmo::TRANSLATE))
							m_GizmoType = ImGuizmo::TRANSLATE;
						ImGui::SameLine();
						if (ImGui::RadioButton("Rotate", m_GizmoType == ImGuizmo::ROTATE))
							m_GizmoType = ImGuizmo::ROTATE;
						ImGui::SameLine();
						if (ImGui::RadioButton("Scale", m_GizmoType == ImGuizmo::SCALE))
							m_GizmoType = ImGuizmo::SCALE;
						ImGui::DragFloat3("Position ", (float*)&currentEntity->transform.Position, m_DragStep, -1000, 1000);
						ImGui::DragFloat3("Scale ", (float*)&currentEntity->transform.Scale, m_DragStep, 0, 1000);
						ImGui::DragFloat3("Rotation ", (float*)&currentEntity->transform.EulerAngles, m_DragStep, 0, 360);
					}

					std::vector<Component*> components = currentEntity->GetAllComponents();
					for (unsigned int i = 0; i < components.size(); ++i)
					{
						if (components[i]->type == MESH)
						{
							MeshData* meshData = currentEntity->GetComponent<MeshData>();
							Model* model = meshData->model;
							if (model && ImGui::CollapsingHeader("Mesh"))
							{
								ImGui::Text(("Name: " + model->name).c_str());
								ImGui::Text(("ID: " + std::to_string(meshData->model_id)).c_str());

								if (ImGui::CollapsingHeader("Models"))
								{
									for (unsigned int i = 0; i < modelsRef.size(); ++i)
									{
										std::string& modelname = modelsRef[i]->name;
										if (ImGui::Button(modelname.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15)))
										{
											meshData->model_id = modelsRef[i]->id;
											meshData->model = modelsRef[i];
										}

										if (modelsRef[i]->id == model->id)
										{
											ImGui::SameLine();
											ImGui::Text("Selected");
										}
									}
								}

								ImGui::Spacing();
								if (ImGui::Button("Delete Mesh"))
									currentEntity->RemoveComponent<MeshData>();
							}
						}
						else if (components[i]->type == PHYSICS)
						{
							PhysicsBody* physicsBody = currentEntity->GetComponent<PhysicsBody>();
							if (physicsBody && ImGui::CollapsingHeader("Physics Body"))
							{
								if (ImGui::Checkbox("Physics Body Enabled", &physicsBody->enabled))
								{
									if (physicsBody->enabled)
									{
										if (!m_PhysicsSystem->EntityInSystem(currentEntity))
											m_PhysicsSystem->AddObject(currentEntity);
									}
									else if (!physicsBody->enabled)
									{
										m_PhysicsSystem->RemoveObject(currentEntity);
									}
								}

								if (physicsBody->enabled)
								{
									ImGui::DragFloat("Physics Mass", &physicsBody->Mass, m_DragStep);
									ImGui::DragFloat3("Physics Velocity", (float*)&physicsBody->Velocity, m_DragStep);
									ImGui::DragFloat3("Physics Force", (float*)&physicsBody->Force, m_DragStep);
								}

								ImGui::Spacing();
								if (ImGui::Button("Delete Physics Body"))
									currentEntity->RemoveComponent<PhysicsBody>();
							}
						}
						else if (components[i]->type == COLLIDER)
						{
							Collider* collider = currentEntity->GetComponent<Collider>();
							if (collider && ImGui::CollapsingHeader("Collider"))
							{
								ImGui::Checkbox("Collider Enabled", &collider->enabled);
								if (collider->enabled)
								{
									ImGui::DragFloat3("Collider Position", (float*)&collider->Position, m_DragStep, -1000, 1000);
									ImGui::DragFloat3("Collider Scale", (float*)&collider->Scale, m_DragStep, 0, 1000);
								}

								ImGui::Spacing();
								if (ImGui::Button("Delete Collider"))
									currentEntity->RemoveComponent<Collider>();
							}
						}
					}

					if (ImGui::CollapsingHeader("Add Component"))
					{
						if (!currentEntity->GetComponent<MeshData>() && ImGui::Button("Mesh"))
						{
							MeshData* new_mesh_data = new MeshData(modelsRef[0], modelsRef[0]->id);
							currentEntity->AddComponent<MeshData>(new_mesh_data);
						}
						if (!currentEntity->GetComponent<Collider>() && ImGui::Button("Collider"))
						{
							Collider* new_collider = new Collider(currentEntity->transform.Position, currentEntity->transform.Scale);
							currentEntity->AddComponent<Collider>(new_collider);
							if (!m_PhysicsSystem->EntityInSystem(currentEntity))
								m_PhysicsSystem->AddObject(currentEntity);
						}
						if (!currentEntity->GetComponent<PhysicsBody>() && ImGui::Button("Physics Body"))
						{
							PhysicsBody* new_physics_body = new PhysicsBody(1.0f);
							currentEntity->AddComponent<PhysicsBody>(new_physics_body);
							if (!m_PhysicsSystem->EntityInSystem(currentEntity))
								m_PhysicsSystem->AddObject(currentEntity);
						}
					}

					ImGui::Spacing();
					if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						// LATER ADD A CONFIRM DELETE MENU CAUSE THIS CAN CAUSE ISSUES
						if (entitiesRef.size() > 1)
						{
							auto itr = std::find(entitiesRef.begin(), entitiesRef.end(), currentEntity); // find in vector
							if (itr == entitiesRef.end()) return; // check if entity is in vector
							entitiesRef.erase(itr); // remove from vector
							m_PropertiesObject.modelptr = nullptr;
							m_PropertiesObject.entityptr = nullptr;
							m_PropertiesObject.id = 0;
						}
						else
							Log::Error("Need at least 1 object in the scene");
					}
				}
			}
			ImGui::EndGroup();
		}
		ImGui::End();
		// ------------------------------

		if (m_AddModelWindowOpen || m_AddEntityWindowOpen)
			m_ViewportFocused = false;
		else
			m_ViewportFocused = true;

		// -----ADD ENTITY WINDOW--------
		if (m_AddEntityWindowOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(window.GetWidth() / 2 - 200, window.GetHeight() / 2 - 200), ImGuiCond_Once);
			ImGui::Begin("Add Entity", &m_AddEntityWindowOpen, popup_flags);

			ImGui::InputText("Name", &newObjProps.name);

			if (ImGui::CollapsingHeader("Model"))
			{
				for (unsigned int i = 0; i < modelsRef.size(); ++i)
				{
					std::string& modelname = modelsRef[i]->name;
					if (ImGui::Button(modelname.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15)))
					{
						newObjProps.modelid = modelsRef[i]->id;
					}
				}
			}

			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::InputFloat3("Position", (float*)&newObjProps.pos);
				ImGui::InputFloat3("Scale", (float*)&newObjProps.scale);
				ImGui::InputFloat3("Euler Angles", (float*)&newObjProps.eulerangles);
				ImGui::InputFloat("Angle", &newObjProps.angle);
			}
			ImGui::Checkbox("PhysicsBody Enabled", &newObjProps.physicsflag);
			if (newObjProps.physicsflag && ImGui::CollapsingHeader("PhysicsBody"))
			{
				ImGui::InputFloat("Mass", &newObjProps.mass);
				ImGui::InputFloat3("Velocity", (float*)&newObjProps.velocity);
				ImGui::InputFloat3("Force", (float*)&newObjProps.force);
			}

			newObjProps.id = entitiesRef.size() + 20000001;

			ImGui::Spacing();
			if (ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
			{
				Model* model = nullptr;
				for (unsigned int i = 0; i < (*m_Models).size(); ++i)
				{
					if ((*m_Models)[i]->id == newObjProps.modelid)
					{
						model = (*m_Models)[i];
					}
				}

				if (model != nullptr && (newObjProps.modelid > 30000000 && newObjProps.modelid < 40000000) && (newObjProps.id > 20000000 && newObjProps.id < 30000000))
				{
					Entity* entity = new Entity(Transform(newObjProps.pos, newObjProps.scale, newObjProps.eulerangles),
						newObjProps.name, newObjProps.id);
					entity->saved = false;

					MeshData* meshData = new MeshData(model, newObjProps.modelid);
					entity->AddComponent<MeshData>(meshData);

					m_Entities->push_back(entity);
				}
				else
					Log::Error("Error creating entity -- model id: " + std::to_string(newObjProps.modelid) + " -- object id: " + std::to_string(newObjProps.id));
			}

			ImGui::End();
		}
		// ------------------------------

		// ---ADD MODEL WINDOW-----------
		if (m_AddModelWindowOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(window.GetWidth() / 2 - 200, window.GetHeight() / 2 - 200), ImGuiCond_Once);
			ImGui::Begin("Add Model", &m_AddModelWindowOpen, popup_flags);

			ImGui::InputText("Name", &newModelProps.name);

			if (ImGui::RadioButton("OBJ", m_LoadModelType == MODEL_TYPE::OBJ))
				m_LoadModelType = MODEL_TYPE::OBJ;
			ImGui::SameLine();
			if (ImGui::RadioButton("DAE", m_LoadModelType == MODEL_TYPE::DAE))
				m_LoadModelType = MODEL_TYPE::DAE;

			if (ImGui::Button("Model File Path", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
			{
				m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + m_ProjectPath;
				m_FilenameModel = std::string(MAX_PATH, '\0');

				OPENFILENAMEA ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFile = (LPSTR)m_FilenameModel.c_str();
				ofn.nMaxFile = m_FilenameModel.size();
				ofn.lpstrInitialDir = m_MyDocuments.c_str();
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

				if (m_LoadModelType == MODEL_TYPE::OBJ)
				{
					ofn.lpstrFilter = "Obj Files\0*.obj\0Any file\0*.obj*\0";
					ofn.lpstrTitle = "Select .obj file";
				}
				else if (m_LoadModelType == MODEL_TYPE::DAE)
				{
					ofn.lpstrFilter = "Dae Files\0*.dae\0Any file\0*.dae*\0";
					ofn.lpstrTitle = "Select .dae file";
				}

				if (GetOpenFileNameA(&ofn))
				{
					m_FilenameModel.resize(m_FilenameModel.find('\0'));

					newModelProps.path = m_FilenameModel;
					Log::Info("You chose the file: " + m_FilenameModel);
				}
				else
					Log::Info("You chose to cancel the file operation");
			}
			ImGui::SameLine();
			ImGui::Text(newModelProps.path.c_str());

			if (ImGui::CollapsingHeader("Textures"))
			{
				ImGui::InputInt("Diffuse Texture Amount", (int*)&newModelProps.num_diffuse);
				ImGui::InputInt("Specular Texture Amount", (int*)&newModelProps.num_specular);
				ImGui::InputInt("Normal Texture Amount", (int*)&newModelProps.num_normal);
				ImGui::InputInt("Height Texture Amount", (int*)&newModelProps.num_height);
			}

			newModelProps.id = modelsRef.size() + 30000001;
			std::vector<unsigned int> numTextures = { newModelProps.num_diffuse, newModelProps.num_specular, newModelProps.num_normal, newModelProps.num_height };

			ImGui::Spacing();
			if (ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
			{
				if (newModelProps.id > 30000000 && newModelProps.id < 40000000)
				{
					Model* model = new Model(newModelProps.path, numTextures, newModelProps.id);
					model->name = newModelProps.name;
					model->saved = false;
					m_Models->push_back(model);
				}
				else
					Log::Error("Error creating model -- model id: " + newModelProps.id);
			}

			ImGui::End();
		}
		// ------------------------------

		// VIEWPORT
		ImGui::SetNextWindowSize(ImVec2((float)viewport.GetWidth(), (float)viewport.GetHeight()), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2((float)viewport.GetXOffset(), 0), ImGuiCond_Always);
		
		app.GetImGuiLayer()->SetStyleClear();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		m_ViewportFocused = m_ViewportHovered; // temporary for editing reasons
		app.GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// Gizmos
		Entity* selectedEntity = m_PropertiesObject.entityptr;
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(camera->isOrthographic);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			const glm::mat4& cameraView = camera->GetViewMatrix();
			const glm::mat4& cameraProjection = camera->GetProjectionMatrix((float)viewport.GetWidth(), (float)viewport.GetHeight());

			// Entity transform
			Transform& tc = selectedEntity->transform;
			glm::mat4 transform = tc.GetTransformMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
				(ImGuizmo::OPERATION)m_GizmoType, (ImGuizmo::MODE)m_GizmoSpace, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				DecomposeTransform(transform, translation, rotation, scale);
				tc.Position = translation;
				tc.EulerAngles = glm::degrees(rotation);
				tc.Scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();
		app.GetImGuiLayer()->SetCustomStyle();

		if (m_GUIFontLoaded)
		{
			ImGui::PopFont();
			m_GUIFontLoaded = false;
		}
	}

	// yoinked from the imgui image example https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-opengl-users
	bool EngineLayer::LoadGUITextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
	{
		// Load from file
		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL)
			return false;

		// Create a OpenGL texture identifier
		GLuint image_texture;
		glGenTextures(1, &image_texture);
		glBindTexture(GL_TEXTURE_2D, image_texture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Upload pixels into texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		stbi_image_free(image_data);

		*out_texture = image_texture;
		*out_width = image_width;
		*out_height = image_height;

		return true;
	}
}