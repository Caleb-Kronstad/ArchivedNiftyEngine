#include "nfpch.h"
#include "EngineLayer.h"

namespace Nifty
{
	EngineLayer::EngineLayer() : Layer("Engine")
	{
		m_ProjectPath = std::filesystem::current_path().generic_string();
		m_Scene = new Scene(m_ProjectPath + "/Assets/Scenes/testscene.nifty", "Test");

		m_PhysicsSystem = new PhysicsSystem();

		camera = new Camera(glm::vec3(0.0f, 2.0f, 3.0f));
		player = new PlayerController(*camera);

		m_Shaders = new std::vector<Shader*>;
		m_Models = new std::vector<Model*>;
		m_Entities = new std::vector<Entity*>;

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

		// Initialize lighting
		DirectionalLight* dirl = new DirectionalLight(glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(0.0f));

		std::vector<PointLight*>* pointl = new std::vector<PointLight*>();
		pointl->push_back(new PointLight(glm::vec3(-3.0f, 2.5f, -1.5f), glm::vec3(0.0f), glm::vec3(1.0f)));
		pointl->push_back(new PointLight(glm::vec3(3.0f, 2.5f, 1.5f), glm::vec3(0.0f), glm::vec3(1.0f)));

		std::vector<SpotLight*>* spotl = new std::vector<SpotLight*>();
		spotl->push_back(new SpotLight(glm::vec3(500), glm::vec3(500), glm::vec3(0.25f), glm::vec3(1.0f)));

		m_Lighting = new Lighting(dirl, pointl, spotl);

		// Initialize shaders
		Shader* texlitShader = new Shader("Assets/Shaders/texture-lit.vert", "Assets/Shaders/texture-lit.frag");
		Shader* shadowtexlitShader = new Shader("Assets/Shaders/shadow-texture-lit.vert", "Assets/Shaders/shadow-texture-lit.frag");
		Shader* texunlitShader = new Shader("Assets/Shaders/texture-unlit.vert", "Assets/Shaders/texture-unlit.frag");
		Shader* lightsourceShader = new Shader("Assets/Shaders/lightsource.vert", "Assets/Shaders/lightsource.frag");
		Shader* skyboxShader = new Shader("Assets/Shaders/skybox.vert", "Assets/Shaders/skybox.frag");
		Shader* shadowmapShader = new Shader("Assets/Shaders/shadowmap.vert", "Assets/Shaders/shadowmap.frag");
		Shader* pointshadowmapShader = new Shader("Assets/Shaders/pointshadowmap.vert", "Assets/Shaders/pointshadowmap.frag", "Assets/Shaders/pointshadowmap.geom");
		//Shader* debugdepthShader = new Shader("Shaders/debugdepth.vert", "Shaders/debugdepth.frag"); // for debugging depth
		//Shader* waterShader = new Shader("Shaders/water.vert", "Shaders/water.frag"); // DOESN'T WORK YET :((((

		m_Shaders->push_back(texlitShader);
		m_Shaders->push_back(shadowtexlitShader);
		m_Shaders->push_back(texunlitShader);
		m_Shaders->push_back(lightsourceShader);
		m_Shaders->push_back(skyboxShader);
		m_Shaders->push_back(shadowmapShader);
		m_Shaders->push_back(pointshadowmapShader);
		//m_Shaders->push_back(debugdepthShader);
		//m_Shaders->push_back(waterShader);

		m_Scene->Load(m_Entities, m_Models, m_PhysicsSystem);

		bool fileImage = LoadGUITextureFromFile("Assets/Textures/GUI/FileImage.png", &m_FileImageTexture, &m_FileImageWidth, &m_FileImageHeight);
		if (m_FileImageTexture == 0)
			Log::Error("Error Loading File Image at: Assets/Textures/GUI/FileImage.png");

		m_Shadows->Generate("Assets/Models/Plane/diffuse_0.png");

		std::vector<Shader*>& shadersRef = *m_Shaders;
		shadersRef[SHADOW_TEXTURE_LIT]->Use();
		shadersRef[SHADOW_TEXTURE_LIT]->SetInt("pointShadowMap", 1);
		shadersRef[SKYBOX]->Use();
		shadersRef[SKYBOX]->SetInt("skybox", 0);
	}

	void EngineLayer::OnDetach()
	{
		m_Scene->Save(m_Entities, m_Models);
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
		glm::vec3& testShadowLightPos = (*m_Lighting->pointLights)[0]->Position;

		m_Shadows->LoadInScene(*shadersRef[POINT_SHADOW_MAP], testShadowLightPos);
		m_Shadows->Set(*shadersRef[POINT_SHADOW_MAP], testShadowLightPos);
		RenderScene(*shadersRef[POINT_SHADOW_MAP]);
		m_Shadows->Reset(true);

		// RESET VIEWPORT
		glViewport(viewport.GetXOffset(), viewport.GetYOffset(), viewport.GetWidth(), viewport.GetHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera->GetProjectionMatrix((float)viewport.GetWidth(), (float)viewport.GetHeight());
		glm::mat4 view = camera->GetViewMatrix();

		shadersRef[SHADOW_TEXTURE_LIT]->Use();
		shadersRef[SHADOW_TEXTURE_LIT]->SetMat4("projection", projection);
		shadersRef[SHADOW_TEXTURE_LIT]->SetMat4("view", view);
		shadersRef[SHADOW_TEXTURE_LIT]->SetVec3("viewPos", camera->Position);

		// SET VALUES FOR SHADOWS
		//shadersRef[SHADOW_TEXTURE_LIT]->SetMat4("lightSpaceMatrix", m_Shadows->lightSpaceMatrix);
		shadersRef[SHADOW_TEXTURE_LIT]->SetFloat("far_plane", m_Shadows->far_plane);

		// SET LIGHTING VALUES
		m_Lighting->SetLighting(*shadersRef[SHADOW_TEXTURE_LIT]);

		m_Shadows->Draw(); // Draw shadow map

		// RENDER SCENE
		RenderScene(*shadersRef[SHADOW_TEXTURE_LIT], true);

		// DRAW LIGHT SOURCES
		shadersRef[LIGHTSOURCE]->Use();
		shadersRef[LIGHTSOURCE]->SetMat4("projection", projection);
		shadersRef[LIGHTSOURCE]->SetMat4("view", view);
		m_Lighting->DrawLightSources(*shadersRef[LIGHTSOURCE], *m_Matrix, *modelsRef[2], *modelsRef[2]);

		// DRAW SKYBOX LAST
		glDepthFunc(GL_LEQUAL);
		shadersRef[SKYBOX]->Use();
		view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
		shadersRef[SKYBOX]->SetMat4("view", view);
		shadersRef[SKYBOX]->SetMat4("projection", projection);
		m_Skybox->RenderSkybox();
		glDepthFunc(GL_LESS);
	}

	void EngineLayer::OnEvent(Event& e)
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();

		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& ev = (KeyPressedEvent&)e;

			// run game
			if (ev.GetKeyCode() == Key::F5)
			{
				if (app.GetGameRunState() == true)
				{
					m_Scene->Load(m_Entities, m_Models, m_PhysicsSystem);
					app.GetGameRunState() = false;
				}
				else if (app.GetGameRunState() == false)
				{
					app.GetGameRunState() = true;
				}
			}

			if (ev.GetKeyCode() == Key::LeftControl)
			{
				CTRL_DOWN = true;
			}

			if (ev.GetKeyCode() == Key::S)
			{
				if (CTRL_DOWN && app.GetGameRunState() == false)
					m_Scene->Save(m_Entities, m_Models);
			}

			// sprinting
			if (ev.GetKeyCode() == Key::LeftShift)
			{
				camera->sprinting = true;
				camera->MovementSpeed *= 3;
			}
			// flying
			if (ev.GetKeyCode() == Key::LeftControl)
			{
				camera->flying = !camera->flying;
			}
		}
		else if (e.GetEventType() == EventType::KeyReleased)
		{
			KeyReleasedEvent& ev = (KeyReleasedEvent&)e;

			if (ev.GetKeyCode() == Key::LeftControl)
			{
				CTRL_DOWN = false;
			}

			// stop sprinting
			if (ev.GetKeyCode() == Key::LeftShift)
			{
				camera->sprinting = false;
				camera->MovementSpeed /= 3;
			}
		}

		else if (e.GetEventType() == EventType::MouseButtonPressed)
		{
			MouseButtonPressedEvent& ev = (MouseButtonPressedEvent&)e;

			if (ev.GetMouseButton() == Key::Button1)
			{
				if (!m_AddEntityWindowOpen && !m_AddModelWindowOpen)
					camera->mouseLocked = true;
			}
		}
		else if (e.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleasedEvent& ev = (MouseButtonReleasedEvent&)e;

			if (ev.GetMouseButton() == Key::Button1)
			{
				if (!m_AddEntityWindowOpen && !m_AddModelWindowOpen)
					camera->mouseLocked = false;
			}

		}

		else if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& ev = (MouseMovedEvent&)e;

			float xpos = static_cast<float>(ev.GetX());
			float ypos = static_cast<float>(ev.GetY());

			if (camera->firstMouse)
			{
				camera->lastX = xpos;
				camera->lastY = ypos;
				camera->firstMouse = false;
			}

			float xoffset = xpos - camera->lastX;
			float yoffset = ypos - camera->lastY;

			camera->lastX = xpos;
			camera->lastY = ypos;

			if (camera->mouseLocked && m_ViewportFocused)
				camera->ProcessMouseMovement(xoffset, yoffset);
		}

		else if (e.GetEventType() == EventType::MouseScrolled)
		{
			MouseScrolledEvent& ev = (MouseScrolledEvent&)e;

			// changes fov based on mouse wheel scroll, but that can get kinda annoying
			//camera->ProcessMouseScroll(static_cast<float>(ev.GetYOffset()));
		}
	}

	void EngineLayer::OnConstantEvent()
	{
		Application& app = Application::Get();
		GLFWwindow* window = app.GetWindow().GetNativeWindow();

		// -- MOVEMENT --
		if (!CTRL_DOWN)
		{
			if (glfwGetKey(window, Key::W) == GLFW_PRESS)
			{
				player->ProcessKeyboard(*camera, FORWARD, app.deltaTime);

				if (camera->flying)
					camera->ProcessKeyboard(FORWARD, app.deltaTime, camera->Front, camera->Right);
				else
					camera->ProcessKeyboard(FORWARD, app.deltaTime, player->Front, player->Right);
			}
			if (glfwGetKey(window, Key::S) == GLFW_PRESS) {
				player->ProcessKeyboard(*camera, BACKWARD, app.deltaTime);

				if (camera->flying)
					camera->ProcessKeyboard(BACKWARD, app.deltaTime, camera->Front, camera->Right);
				else
					camera->ProcessKeyboard(BACKWARD, app.deltaTime, player->Front, player->Right);
			}
			if (glfwGetKey(window, Key::A) == GLFW_PRESS) {
				player->ProcessKeyboard(*camera, LEFT, app.deltaTime);

				if (camera->flying)
					camera->ProcessKeyboard(LEFT, app.deltaTime, camera->Front, camera->Right);
				else
					camera->ProcessKeyboard(LEFT, app.deltaTime, player->Front, player->Right);
			}
			if (glfwGetKey(window, Key::D) == GLFW_PRESS) {
				player->ProcessKeyboard(*camera, RIGHT, app.deltaTime);

				if (camera->flying)
					camera->ProcessKeyboard(RIGHT, app.deltaTime, camera->Front, camera->Right);
				else
					camera->ProcessKeyboard(RIGHT, app.deltaTime, player->Front, player->Right);
			}
		}
		// ----
	}
}