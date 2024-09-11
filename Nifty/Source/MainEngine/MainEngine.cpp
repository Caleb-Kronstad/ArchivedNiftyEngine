#include "nfpch.h"
#include "MainEngine.h"

namespace Nifty
{
	EngineLayer::EngineLayer() : Layer("Engine")
	{
		m_ProjectPath = std::filesystem::current_path().generic_string();
		m_Scene = new Scene(m_ProjectPath + "/Assets/Scenes/testscene.nifty", "Test");

		m_PhysicsSystem = new PhysicsSystem();

		m_Shaders = new std::vector<Shader*>;
		m_Models = new std::vector<Model*>;
		m_Entities = new std::vector<Entity*>;
		m_Cameras = new std::vector<Camera*>;

		Camera* engine_camera = new Camera(glm::vec3(0.0f, 3.0f, 0.0f));
		Camera* test_game_camera = new Camera(glm::vec3(0.0f, 3.0f, 10.0f));
		m_Cameras->push_back(engine_camera);
		m_Cameras->push_back(test_game_camera);
		m_CurrentCamera = engine_camera;

		m_Matrix = new glm::mat4(1.0f);
	}

	EngineLayer::~EngineLayer()
	{
	}

	void EngineLayer::OnAttach()
	{
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		Window& viewport = app.GetViewport();

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Initialize font
		std::string openSansPath = "Assets/Fonts/OpenSans/OpenSans-Regular.ttf";
		m_GUIFont = io.Fonts->AddFontFromFileTTF(openSansPath.c_str(), 16.0f);
		m_MenuFont = io.Fonts->AddFontFromFileTTF(openSansPath.c_str(), 18.0f);

		m_Skybox = new Skybox();
		m_Shadows = new Shadows();
		m_Lighting = new LightingSystem();

		m_Scene->Load(m_Entities, m_Models, m_Shaders, m_PhysicsSystem, m_Lighting);

		bool fileImage = LoadGUITextureFromFile("Assets/Textures/GUI/FileImage.png", &m_FileImageTexture, &m_FileImageWidth, &m_FileImageHeight);
		if (m_FileImageTexture == 0)
			Log::Error("Error Loading File Image at: Assets/Textures/GUI/FileImage.png");

		m_Shadows->Generate("Assets/Models/Plane/diffuse_0.png");

		std::vector<Shader*>& shadersRef = *m_Shaders;
		shadersRef[SHADOW_TEXTURE_LIT]->Use();
		shadersRef[SHADOW_TEXTURE_LIT]->SetInt("pointShadowMap", 1);
		shadersRef[SKYBOX]->Use();
		shadersRef[SKYBOX]->SetInt("skybox", 0);

		m_ProjectFiles = GetFilesInDirectory("Assets");
		m_CurrentProjectFilePath = std::filesystem::path("Assets");
	}

	void EngineLayer::OnDetach()
	{
		m_Scene->Save(m_Entities, m_Models, m_Shaders);
	}

	void EngineLayer::OnUpdate()
	{
		Application& app = Application::Get();
		Window& window = app.GetWindow();
		Window& viewport = app.GetViewport();

		std::vector<Shader*>& shadersRef = *m_Shaders;
		std::vector<Model*>& modelsRef = *m_Models;
		std::vector<Entity*>& entitiesRef = *m_Entities;

		if (m_ViewportFocused)
			OnConstantEvent();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// SHADOWS
		if (m_Lighting->pointLights->size() > 0) {
			glm::vec3& testShadowLightPos = (*m_Lighting->pointLights)[0]->Position;
			m_Shadows->LoadInScene(*shadersRef[POINT_SHADOW_MAP], testShadowLightPos);
			m_Shadows->Set(*shadersRef[POINT_SHADOW_MAP], testShadowLightPos);
			for (Entity* entity : entitiesRef)
			{
				if (entity->has_shadows)
					entity->Draw(*shadersRef[POINT_SHADOW_MAP], *m_Matrix);
			}
			m_Shadows->Reset(true);
		}

		// RESET VIEWPORT
		glViewport(viewport.GetXOffset(), viewport.GetYOffset(), viewport.GetWidth(), viewport.GetHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = m_CurrentCamera->GetProjectionMatrix((float)viewport.GetWidth(), (float)viewport.GetHeight());
		glm::mat4 view = m_CurrentCamera->GetViewMatrix();

		if (app.GetGameRunState())
			m_PhysicsSystem->StepPhysics(app.deltaTime);

		// RENDER SCENE
		for (Shader* shader : shadersRef)
		{
			shader->Use();
			shader->SetMat4("projection", projection);
			shader->SetMat4("view", view);

			if (shader->id == 40000001) // basic shader
			{
				shader->SetVec3("viewPos", m_CurrentCamera->Position);

				// SET VALUES FOR SHADOWS
				//shader->SetMat4("lightSpaceMatrix", m_Shadows->lightSpaceMatrix);
				shader->SetFloat("far_plane", m_Shadows->far_plane);

				m_Lighting->SetLighting(*shader);
				m_Shadows->Draw(); // Draw shadow map
			}

			for (Entity* entity : shader->GetEntities())
			{
				if (shader->id == 40000007) // animation shader
				{
					SkinnedMesh* skinned_mesh = entity->GetComponent<SkinnedMesh>();
					if (skinned_mesh)
					{
						/*skinned_mesh->Update(app.deltaTime);
						std::vector<glm::mat4> boneTransforms = skinned_mesh->GetBoneTransforms();
						for (unsigned int i = 0; i < boneTransforms.size(); ++i)
							shader->SetMat4("boneTransforms[" + std::to_string(i) + "]", boneTransforms[i]);*/
					}
				}
				
				entity->Draw(*shader, *m_Matrix);
			}
		}

		// DRAW SKYBOX LAST
		glDepthFunc(GL_LEQUAL);
		shadersRef[SKYBOX]->Use();
		view = glm::mat4(glm::mat3(m_CurrentCamera->GetViewMatrix()));
		shadersRef[SKYBOX]->SetMat4("view", view);
		shadersRef[SKYBOX]->SetMat4("projection", projection);
		m_Skybox->RenderSkybox();
		glDepthFunc(GL_LESS);
	}

	void EngineLayer::OnEvent(Event& e)
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();

		// run game
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& ev = (KeyPressedEvent&)e;

			if (ev.GetKeyCode() == Key::F5)
			{
				if (app.GetGameRunState() == true)
					EndGame();
				else if (app.GetGameRunState() == false)
					StartGame();
			}
		}

		if (app.GetGameRunState() == true) return;

		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& ev = (KeyPressedEvent&)e;

			if (ev.GetKeyCode() == Key::LeftControl)
				CTRL_DOWN = true;

			if (ev.GetKeyCode() == Key::S)
			{
				if (CTRL_DOWN && app.GetGameRunState() == false)
					m_Scene->Save(m_Entities, m_Models, m_Shaders);
			}

			// sprinting
			if (ev.GetKeyCode() == Key::LeftShift)
			{
				m_CurrentCamera->sprinting = true;
				m_CurrentCamera->MovementSpeed *= 3;
			}
		}
		else if (e.GetEventType() == EventType::KeyReleased)
		{
			KeyReleasedEvent& ev = (KeyReleasedEvent&)e;

			if (ev.GetKeyCode() == Key::LeftControl)
				CTRL_DOWN = false;

			// stop sprinting
			if (ev.GetKeyCode() == Key::LeftShift)
			{
				m_CurrentCamera->sprinting = false;
				m_CurrentCamera->MovementSpeed /= 3;
			}
		}

		else if (e.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressedEvent& ev = (MouseButtonPressedEvent&)e;

			if (ev.GetMouseButton() == Key::Button1)
			{
				if (m_ViewportFocused)
					m_CurrentCamera->mouseLocked = true;
			}
		}
		else if (e.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleasedEvent& ev = (MouseButtonReleasedEvent&)e;

			if (ev.GetMouseButton() == Key::Button1)
			{
				if (m_ViewportFocused)
					m_CurrentCamera->mouseLocked = false;
			}

		}

		else if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& ev = (MouseMovedEvent&)e;

			float xpos = static_cast<float>(ev.GetX());
			float ypos = static_cast<float>(ev.GetY());

			if (m_CurrentCamera->firstMouse)
			{
				m_CurrentCamera->lastX = xpos;
				m_CurrentCamera->lastY = ypos;
				m_CurrentCamera->firstMouse = false;
			}

			float xoffset = xpos - m_CurrentCamera->lastX;
			float yoffset = ypos - m_CurrentCamera->lastY;

			m_CurrentCamera->lastX = xpos;
			m_CurrentCamera->lastY = ypos;

			if (m_CurrentCamera->mouseLocked && m_ViewportFocused)
				m_CurrentCamera->ProcessMouseMovement(xoffset, yoffset);
		}

		else if (e.GetEventType() == EventType::MouseScrolled)
		{
			MouseScrolledEvent& ev = (MouseScrolledEvent&)e;

			// changes fov based on mouse wheel scroll, but that can get kinda annoying
			//m_CurrentCamera->ProcessMouseScroll(static_cast<float>(ev.GetYOffset()));
		}
	}

	void EngineLayer::OnConstantEvent()
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();

		if (app.GetGameRunState() == true) return;

		// -- MOVEMENT --
		if (!CTRL_DOWN)
		{
			if (glfwGetKey(window, Key::W) == GLFW_PRESS)
				m_CurrentCamera->ProcessKeyboard(FORWARD, app.deltaTime);
			if (glfwGetKey(window, Key::S) == GLFW_PRESS)
				m_CurrentCamera->ProcessKeyboard(BACKWARD, app.deltaTime);
			if (glfwGetKey(window, Key::A) == GLFW_PRESS)
				m_CurrentCamera->ProcessKeyboard(LEFT, app.deltaTime);
			if (glfwGetKey(window, Key::D) == GLFW_PRESS)
				m_CurrentCamera->ProcessKeyboard(RIGHT, app.deltaTime);
		}
		// ----
	}

	bool EngineLayer::StartGame()
	{
		Application& app = Application::Get();

		m_CurrentCamera = (*m_Cameras)[1];
		app.GetGameRunState() = true;

		return true;
	}

	bool EngineLayer::EndGame()
	{
		Application& app = Application::Get();

		m_Scene->LoadWithoutShaders(m_Entities, m_Models, m_PhysicsSystem, m_Lighting);
		m_CurrentCamera = (*m_Cameras)[0];
		app.GetGameRunState() = false;

		return true;
	}
}