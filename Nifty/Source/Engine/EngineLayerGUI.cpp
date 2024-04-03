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
		std::vector<GameObject*>& objectsRef = *m_GameObjects;

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
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;

		if (!m_GUIFontLoaded)
		{
			ImGui::PushFont(m_GUIFont);
			m_GUIFontLoaded = true;
		}

		// -----MENU BAR-----------------
		ImGui::SetNextWindowSize(ImVec2(window.GetWidth(), window.GetHeight() / 20), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::Begin("Menu", new bool(true), menu_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save Scene"))
				{
					SaveScene(m_Scene);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Add Object", NULL, &m_AddObjectWindowOpen))
					m_AddModelWindowOpen = false;
				if (ImGui::MenuItem("Add Model", NULL, &m_AddModelWindowOpen))
					m_AddObjectWindowOpen = false;
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
		ImGui::Begin("File Manager", new bool(false), window_flags);

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
		ImGui::Begin("Explorer", new bool(false), window_flags);

		for (unsigned int i = 0; i < objectsRef.size(); ++i)
		{
			if (ImGui::Button(objectsRef[i]->name.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15))) // checks if button is pressed (and if it's allowed to be pressed)
			{
				m_PropertiesObject.id = objectsRef[i]->id;
				m_PropertiesObject.objectptr = objectsRef[i];
			}
					
		}

		ImGui::End();
		// ------------------------------


		// -----PROPERTIES---------------
		ImGui::SetNextWindowSize(ImVec2(window.GetWidth() - viewport.GetWidth(), window.GetHeight() / 2), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(0, window.GetHeight() / 2), ImGuiCond_Always);
		ImGui::Begin("Properties", new bool(false), window_flags);

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

					if (ImGui::Button("Change File", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + "\\Documents";
						m_FilenameModel = std::string(MAX_PATH, '\0');

						OPENFILENAMEA ofn;
						ZeroMemory(&ofn, sizeof(OPENFILENAME));
						ofn.lStructSize = sizeof(OPENFILENAME);
						ofn.lpstrFilter = "Obj Files\0*.obj\0Any file\0*.obj*\0";
						ofn.lpstrFile = (LPSTR)m_FilenameModel.c_str();
						ofn.nMaxFile = m_FilenameModel.size();
						ofn.lpstrInitialDir = m_MyDocuments.c_str();
						ofn.lpstrTitle = "Select .obj file";
						ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

						if (GetOpenFileNameA(&ofn))
						{
							m_FilenameModel.resize(m_FilenameModel.find('\0'));

							currentModel->model_path = m_FilenameModel;
							std::cout << "You chose the file: " << m_FilenameModel << "\n";
						}
						else
						{
							std::cout << "You chose to cancel the file operation.\n";
						}
					}
					ImGui::SameLine();
					ImGui::Text(currentModel->model_path.c_str());

					if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						// LATER ADD A CONFIRM DELETE MENU CAUSE THIS CAN CAUSE ISSUES
						for (unsigned int i = 0; i < modelsRef.size(); ++i)
						{
							if (modelsRef[i] == currentModel)
							{
								if (modelsRef.size() > 1)
								{
									m_GameObjects->erase(m_GameObjects->begin() + i);
									m_PropertiesObject.id = 0;
									m_PropertiesObject.modelptr = nullptr;
									m_PropertiesObject.objectptr = nullptr;
								}
								else
									Log::Error("Need at least 1 object in the scene! (For now cause im shit at coding)");
							}
						}
					}
				}
			}
			// GAME OBJECT PROPERTY PAGE
			if (m_PropertiesObject.id > 20000000 && m_PropertiesObject.id < 30000000)
			{
				GameObject* currentObject = m_PropertiesObject.objectptr;
				if (currentObject != nullptr)
				{
					std::string name = currentObject->name + " (" + std::to_string(currentObject->id) + ")";

					std::string positionName = "Position";
					std::string scaleName = "Scale";
					std::string eulerAnglesName = "Rotation";
					std::string angleName = "Angle";

					std::string massName = "Mass";
					std::string velocityName = "Velocity";
					std::string forceName = "Force";

					ImGui::Text(name.c_str());

					if (ImGui::CollapsingHeader("Transform"))
					{
						// IMPLEMENT IMGUIZMO SOMEDAY -- NOT DOING IT RIGHT NOW CAUSE ITS A BITCH TO ADD
						/*if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
							m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
						ImGui::SameLine();
						if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
							m_CurrentGizmoOperation = ImGuizmo::SCALE;
						ImGui::SameLine();
						if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
							m_CurrentGizmoOperation = ImGuizmo::ROTATE;

						ImGuizmo::DecomposeMatrixToComponents((float*)&m_GizmoMatrix, (float*)&m_PropertiesObject->transform.Position, (float*)&m_PropertiesObject->transform.EulerAngles, (float*)&m_PropertiesObject->transform.Scale);*/

						ImGui::InputFloat("DragStep", &m_DragStep);
						if (ImGui::RadioButton("Input", m_InputTransformValues))
							m_InputTransformValues = true;
						if (ImGui::RadioButton("Drag", !m_InputTransformValues))
							m_InputTransformValues = false;

						if (m_InputTransformValues)
						{
							ImGui::InputFloat3(positionName.c_str(), (float*)&currentObject->transform.Position);
							ImGui::InputFloat3(scaleName.c_str(), (float*)&currentObject->transform.Scale);
							ImGui::InputFloat3(eulerAnglesName.c_str(), (float*)&currentObject->transform.EulerAngles);
							ImGui::InputFloat(angleName.c_str(), (float*)&currentObject->transform.Angle);
						}
						if (!m_InputTransformValues)
						{
							ImGui::DragFloat3(positionName.c_str(), (float*)&currentObject->transform.Position, m_DragStep, -1000, 1000);
							ImGui::DragFloat3(scaleName.c_str(), (float*)&currentObject->transform.Scale, m_DragStep, 0, 1000);
							ImGui::DragFloat3(eulerAnglesName.c_str(), (float*)&currentObject->transform.EulerAngles, m_DragStep, 0, 1);
							ImGui::SliderFloat(angleName.c_str(), (float*)&currentObject->transform.Angle, 0, 360);
						}

						/*ImGuizmo::RecomposeMatrixFromComponents((float*)&m_PropertiesObject->transform.Position, (float*)&m_PropertiesObject->transform.EulerAngles, (float*)&m_PropertiesObject->transform.Scale, (float*)&m_GizmoMatrix);

						if (m_CurrentGizmoOperation != ImGuizmo::SCALE)
						{
							if (ImGui::RadioButton("Local", m_CurrentGizmoMode == ImGuizmo::LOCAL))
								m_CurrentGizmoMode = ImGuizmo::LOCAL;
							ImGui::SameLine();
							if (ImGui::RadioButton("World", m_CurrentGizmoMode == ImGuizmo::WORLD))
								m_CurrentGizmoMode = ImGuizmo::WORLD;
						}

						ImGui::Checkbox("Snap", &m_UseGizmoSnap);

						ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
						glm::mat4 viewMatrix = camera->GetViewMatrix();
						glm::mat4 projMatrix = camera->GetProjectionMatrix(viewport);
						ImGuizmo::Manipulate((float*)&viewMatrix, (float*)&projMatrix, m_CurrentGizmoOperation, m_CurrentGizmoMode, (float*)m_Matrix, NULL);
						*/
					}

					if (ImGui::CollapsingHeader("Model"))
					{
						for (unsigned int i = 0; i < modelsRef.size(); ++i)
						{
							std::string& modelname = modelsRef[i]->name;
							if (ImGui::Button(modelname.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15)))
							{
								currentObject->modelid = modelsRef[i]->id;
								currentObject->model = modelsRef[i];
							}
						}
					}

					ImGui::Checkbox("PhysicsBody Enabled", &currentObject->transform.PhysicsActive);
					if (currentObject->transform.PhysicsActive && ImGui::CollapsingHeader("PhysicsBody"))
					{
						ImGui::InputFloat(massName.c_str(), &currentObject->transform.Mass);
						ImGui::InputFloat3(velocityName.c_str(), (float*)&currentObject->transform.Velocity);
						ImGui::InputFloat3(forceName.c_str(), (float*)&currentObject->transform.Force);
					}

					if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
					{
						// LATER ADD A CONFIRM DELETE MENU CAUSE THIS CAN CAUSE ISSUES
						for (unsigned int i = 0; i < objectsRef.size(); ++i)
						{
							if (objectsRef[i] == currentObject)
							{
								if (objectsRef.size() > 1)
								{
									m_GameObjects->erase(m_GameObjects->begin() + i);
									m_PropertiesObject.id = 0;
									m_PropertiesObject.modelptr = nullptr;
									m_PropertiesObject.objectptr = nullptr;
								}
								else
									Log::Error("Need at least 1 object in the scene! (For now cause im shit at coding)");
							}
						}
					}
				}
			}
			ImGui::EndGroup();
		}
		ImGui::End();
		// ------------------------------

		if (!m_AddModelWindowOpen && !m_AddObjectWindowOpen)
			m_GameWindowInFocus = true;
		else
			m_GameWindowInFocus = false;

		// -----ADD OBJECT WINDOW--------
		if (m_AddObjectWindowOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(window.GetWidth() / 2 - 200, window.GetHeight() / 2 - 200), ImGuiCond_Once);
			ImGui::Begin("Add Object", &m_AddObjectWindowOpen, popup_flags);

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

			newObjProps.id = objectsRef.size() + 20000001;

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
					GameObject* gameObject = new GameObject(
						Transform(newObjProps.pos, newObjProps.scale, newObjProps.eulerangles, newObjProps.angle,
							newObjProps.physicsflag, newObjProps.velocity, newObjProps.force, newObjProps.mass),
						model, newObjProps.name, newObjProps.id, newObjProps.modelid);
					gameObject->saved = false;
					m_GameObjects->push_back(gameObject);
				}
				else
					Log::Error("Error creating object -- model id: " + std::to_string(newObjProps.modelid) + " -- object id: " + std::to_string(newObjProps.id));
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

			if (ImGui::Button("Model File Path", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
			{
				m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + "\\Documents";
				m_FilenameModel = std::string(MAX_PATH, '\0');

				OPENFILENAMEA ofn;
				ZeroMemory(&ofn, sizeof(OPENFILENAME));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.lpstrFilter = "Obj Files\0*.obj\0Any file\0*.obj*\0";
				ofn.lpstrFile = (LPSTR)m_FilenameModel.c_str();
				ofn.nMaxFile = m_FilenameModel.size();
				ofn.lpstrInitialDir = m_MyDocuments.c_str();
				ofn.lpstrTitle = "Select .obj file";
				ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

				if (GetOpenFileNameA(&ofn))
				{
					m_FilenameModel.resize(m_FilenameModel.find('\0'));

					newModelProps.path = m_FilenameModel;
					std::cout << "You chose the file: " << m_FilenameModel << "\n";
				}
				else
					std::cout << "You chose to cancel the file operation.\n";
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