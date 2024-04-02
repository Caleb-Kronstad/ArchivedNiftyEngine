#include "EngineLayer.h"

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
	ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always);
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
			ImGui::MenuItem("Add Object", NULL, &m_AddObjectWindowOpen);
			ImGui::MenuItem("Add Model", NULL, &m_AddModelWindowOpen);
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

	ImGui::Image((void*)m_FileImageTexture, ImVec2(100, 100));

	ImGui::End();
	// ------------------------------


	// -----EXPLORER-----------------
	ImGui::SetNextWindowSize(ImVec2(window.GetWidth() - viewport.GetWidth(), window.GetHeight() / 2 - (window.GetHeight() / 50)), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, window.GetHeight() / 50), ImGuiCond_Always);
	ImGui::Begin("Explorer", new bool(false), window_flags);

	for (unsigned int i = 0; i < objectsRef.size(); ++i)
	{ 
		bool btn = ImGui::Button(objectsRef[i]->name.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15));
		if (btn && m_ImGuiCanCaptureMouse) // checks if button is pressed (and if it's allowed to be pressed)
			m_PropertiesObject = objectsRef[i];
	}

	ImGui::End();
	// ------------------------------


	// -----PROPERTIES---------------
	ImGui::SetNextWindowSize(ImVec2(window.GetWidth() - viewport.GetWidth(), window.GetHeight() / 2), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0, window.GetHeight() / 2), ImGuiCond_Always);
	ImGui::Begin("Properties", new bool(false), window_flags);

	if (m_PropertiesObject != nullptr)
	{
		ImGui::BeginGroup();

		std::string name = m_PropertiesObject->name + " (" + std::to_string(m_PropertiesObject->id) + ")";

		std::string positionName = "Position (" + std::to_string(m_PropertiesObject->id) + ")";
		std::string scaleName = "Scale (" + std::to_string(m_PropertiesObject->id) + ")";
		std::string eulerAngleName = "Euler Angles (" + std::to_string(m_PropertiesObject->id) + ")";
		std::string angleName = "Angle (" + std::to_string(m_PropertiesObject->id) + ")";

		std::string massName = "Mass (" + std::to_string(m_PropertiesObject->id) + ")";
		std::string velocityName = "Velocity (" + std::to_string(m_PropertiesObject->id) + ")";
		std::string forceName = "Force (" + std::to_string(m_PropertiesObject->id) + ")";

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
				ImGui::InputFloat3(positionName.c_str(), (float*)&m_PropertiesObject->transform.Position);
				ImGui::InputFloat3(scaleName.c_str(), (float*)&m_PropertiesObject->transform.Scale);
				ImGui::InputFloat3(eulerAngleName.c_str(), (float*)&m_PropertiesObject->transform.EulerAngles);
				ImGui::InputFloat(angleName.c_str(), (float*)&m_PropertiesObject->transform.Angle);
			}
			if (!m_InputTransformValues)
			{
				ImGui::DragFloat3(positionName.c_str(), (float*)&m_PropertiesObject->transform.Position, m_DragStep, -1000, 1000);
				ImGui::DragFloat3(scaleName.c_str(), (float*)&m_PropertiesObject->transform.Scale, m_DragStep, 0, 1000);
				ImGui::DragFloat3(eulerAngleName.c_str(), (float*)&m_PropertiesObject->transform.EulerAngles, m_DragStep, 0, 1);
				ImGui::SliderFloat(angleName.c_str(), (float*)&m_PropertiesObject->transform.Angle, 0, 360);
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
		ImGui::Checkbox("PhysicsBody Enabled", &m_PropertiesObject->transform.PhysicsActive);
		if (m_PropertiesObject->transform.PhysicsActive && ImGui::CollapsingHeader("PhysicsBody"))
		{
			ImGui::InputFloat(massName.c_str(), &m_PropertiesObject->transform.Mass);
			ImGui::InputFloat3(velocityName.c_str(), (float*)&m_PropertiesObject->transform.Velocity);
			ImGui::InputFloat3(forceName.c_str(), (float*)&m_PropertiesObject->transform.Force);
		}

		if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)))
		{
			// LATER ADD A CONFIRM DELETE MENU CAUSE THIS CAN CAUSE ISSUES
			for (unsigned int i = 0; i < objectsRef.size(); ++i)
			{
				if (objectsRef[i] == m_PropertiesObject)
				{
					if (objectsRef.size() > 1)
					{
						m_GameObjects->erase(m_GameObjects->begin() + i);
						m_PropertiesObject = nullptr;
					}
					else
						Log::Error("Need at least 1 object in the scene! (For now cause im shit at coding)");
				}
			}
		}

		ImGui::EndGroup();
	}

	ImGui::End();
	// ------------------------------

	// -----ADD OBJECT WINDOW--------
	if (m_AddObjectWindowOpen)
	{
		m_GameWindowInFocus = false;
		ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(window.GetWidth() / 2 - 200, window.GetHeight() / 2 - 200), ImGuiCond_Once);
		ImGui::Begin("Add Object", &m_AddObjectWindowOpen, popup_flags);

		ImGui::InputText("Name", &newObjProps.name);
		
		if (ImGui::CollapsingHeader("Model"))
		{
			for (unsigned int i = 0; i < modelsRef.size(); ++i)
			{
				std::string& modelpath = modelsRef[i]->model_path;
				if (ImGui::Button(modelpath.c_str(), ImVec2(ImGui::GetWindowWidth() * 0.9f, 15)))
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
		if (ImGui::Button("Create", ImVec2(ImGui::GetWindowWidth() * 0.3f, 20)) && m_ImGuiCanCaptureMouse)
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

				SaveScene(m_Scene);
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
		m_GameWindowInFocus = false;
	}
	// ------------------------------

	if (!m_AddModelWindowOpen && !m_AddObjectWindowOpen)
		m_GameWindowInFocus = true;

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