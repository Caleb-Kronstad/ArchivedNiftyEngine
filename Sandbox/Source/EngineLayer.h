#pragma once

#include "Nifty.h"
#include "PlayerController.h"

using namespace Nifty;

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


class EngineLayer : public Layer
{
private:
	Scene* m_Scene = new Scene("Assets/Scenes/testscene.nifty", "Test");

	std::vector<Shader*>* m_Shaders;
	std::vector<Model*>* m_Models;
	std::vector<GameObject*>* m_GameObjects;
	NewObjectProps newObjProps;
	GameObject* m_PropertiesObject = nullptr;

	Skybox* m_Skybox;
	Shadows* m_Shadows;
	Lighting* m_Lighting;

	Camera* camera;
	PlayerController* player;

	glm::mat4* m_Matrix;
	glm::mat4* m_GizmoMatrix;

private: // GUI
	bool m_GameWindowInFocus;
	bool m_ImGuiCanCaptureMouse;

	bool m_InputTransformValues = true;
	float m_DragStep = 1.0f;

	ImFont* m_MenuFont;
	bool m_MenuFontLoaded = false;
	ImFont* m_GUIFont;
	bool m_GUIFontLoaded = false;

	GLuint m_FileImageTexture = 0;
	int m_FileImageWidth = 0;
	int m_FileImageHeight = 0;

	bool m_AddObjectWindowOpen = false;
	bool m_AddModelWindowOpen = false;

	ImGuizmo::OPERATION m_CurrentGizmoOperation;
	ImGuizmo::MODE m_CurrentGizmoMode;
	bool m_UseGizmoSnap = false;

public:
	EngineLayer();
	~EngineLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	void OnEvent(Event& e) override;
	void OnConstantEvent();

	void RenderScene(Shader& shader);
	bool LoadScene(Scene* scene);
	bool SaveScene(Scene* scene);

	static bool LoadGUITextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};