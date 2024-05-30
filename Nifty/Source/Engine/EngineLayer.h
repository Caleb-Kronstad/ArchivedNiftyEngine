#pragma once

#include "Math/Math.h"
#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Camera.h"
#include "Core/Skybox.h"
#include "Core/Shadows.h"
#include "Core/Shader.h"
#include "Core/PlayerController.h"

#include "Components/Components.h"
#include "Components/Transform.h"
#include "Components/PhysicsSystem.h"

#include "Core/SaveSystem.h"
#include "Core/Entity.h"

#include "Scene/Scene.h"

namespace Nifty
{
	enum SHADER_TYPE
	{
		TEXTURE_LIT = 0,
		SHADOW_TEXTURE_LIT = 1,
		TEXTURE_UNLIT = 2,
		LIGHTSOURCE = 3,
		SKYBOX = 4,
		SHADOW_MAP = 5,
		POINT_SHADOW_MAP = 6,
		DEBUG_DEPTH = 7,
		WATER = 8
	};

	struct NewObjectProps
	{
		std::string name = "New Object";
		int modelid = 0;
		int id = 0;

		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 eulerangles = glm::vec3(1.0f);
		float angle = 0;

		bool physicsflag = 0;
		float mass = 0;
		glm::vec3 velocity = glm::vec3(0.0f);
		glm::vec3 force = glm::vec3(0.0f);
	};

	struct NewModelProps
	{
		std::string name = "New Model";
		std::string path = "";
		int id = 0;

		unsigned int num_diffuse = 0;
		unsigned int num_specular = 0;
		unsigned int num_normal = 0;
		unsigned int num_height = 0;

		bool loadAnimations = false;
	};

	struct PropertiesObject
	{
		int id;
		Entity* entityptr = nullptr;
		Model* modelptr = nullptr;
	};

	class EngineLayer : public Layer
	{
	public:
		Scene* m_Scene = new Scene("Assets/Scenes/testscene.nifty", "Test");

		PhysicsSystem* m_PhysicsSystem;

		std::vector<Shader*>* m_Shaders;
		std::vector<Model*>* m_Models;
		std::vector<Entity*>* m_Entities;
		PropertiesObject m_PropertiesObject;

		Skybox* m_Skybox;
		Shadows* m_Shadows;
		Lighting* m_Lighting;

		Camera* camera = nullptr;
		PlayerController* player = nullptr;

		glm::mat4* m_Matrix;

		std::string m_ProjectPath;
		std::string m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + m_ProjectPath;
		std::string m_FilenameModel = std::string(MAX_PATH, '\0');

		bool CTRL_DOWN = false;
		
		MODEL_TYPE m_LoadModelType = MODEL_TYPE::OBJ;

	public: // IMGUI
		bool m_ViewportFocused;
		bool m_ViewportHovered;
		glm::vec2 m_ViewportSize;

		float m_DragStep = 1.0f;

		ImFont* m_MenuFont;
		bool m_MenuFontLoaded = false;
		ImFont* m_GUIFont;
		bool m_GUIFontLoaded = false;

		GLuint m_FileImageTexture = 0;
		int m_FileImageWidth = 0;
		int m_FileImageHeight = 0;

		bool m_AddEntityWindowOpen = false;
		bool m_AddModelWindowOpen = false;
		NewObjectProps newObjProps;
		NewModelProps newModelProps;

		int m_GizmoType = (int)ImGuizmo::TRANSLATE;
		int m_GizmoSpace = 0;

	public:
		EngineLayer();
		~EngineLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		// engine
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		void OnConstantEvent();

		void RenderScene(Shader& shader, const bool& checkCollision = false);
		bool LoadScene(Scene* scene);
		bool SaveScene(Scene* scene);

		static bool LoadGUITextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
	};

}