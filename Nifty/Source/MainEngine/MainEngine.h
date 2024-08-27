#pragma once

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/FileUtility.h"
#include "Core/MathUtility.h"
#include "Core/TextUtility.h"

#include "RenderingEngine/Skybox.h"
#include "RenderingEngine/Shadows.h"
#include "RenderingEngine/Shaders/Shader.h"
#include "RenderingEngine/Components.h"
#include "RenderingEngine/Transform.h"
#include "RenderingEngine/Entity.h"
#include "RenderingEngine/Lighting.h"
#include "RenderingEngine/Scene.h"

#include "PhysicsEngine/PhysicsSystem.h"
#include "PhysicsEngine/Animation/Bone.h"
#include "PhysicsEngine/Animation/Animation.h"
#include "PhysicsEngine/Animation/Animator.h"

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
		ANIM_ONLY = 7,
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
	};

	struct PropertiesObject
	{
		int id;
		Entity* entityptr = nullptr;
		Model* modelptr = nullptr;
		Shader* shaderptr = nullptr;
	};

	class EngineLayer : public Layer
	{
	private:
		std::vector<Camera*>* m_Cameras;
		Camera* m_CurrentCamera = nullptr;

		PhysicsSystem* m_PhysicsSystem;

		std::vector<Shader*>* m_Shaders;
		std::vector<Model*>* m_Models;
		std::vector<Entity*>* m_Entities;
		PropertiesObject m_PropertiesObject;

		std::vector<std::filesystem::path> m_ProjectFiles;
		std::filesystem::path m_CurrentProjectFilePath;

	public:
		Scene* m_Scene = new Scene("Assets/Scenes/testscene.nifty", "Test");

		Skybox* m_Skybox;
		Shadows* m_Shadows;
		LightingSystem* m_Lighting;

		std::string m_ProjectPath;
		std::string m_MyDocuments = std::string(getenv("HOMEDRIVE")) + getenv("HOMEPATH") + m_ProjectPath;
		std::string m_FilenameModel = std::string(MAX_PATH, '\0');

		MODEL_TYPE m_LoadModelType = MODEL_TYPE::OBJ;
		glm::mat4* m_Matrix;

	public: // GUI and other
		bool CTRL_DOWN = false;

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

		static bool LoadGUITextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

		bool StartGame();
		bool EndGame();

	public: // getters
		Camera*& GetCamera() { return m_CurrentCamera; }
		std::vector<Model*>*& GetModels() { return m_Models; }
		std::vector<Entity*>*& GetEntities() { return m_Entities; }
		std::vector<Shader*>*& GetShaders() { return m_Shaders; }
	};

}