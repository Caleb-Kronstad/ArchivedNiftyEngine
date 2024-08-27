/*
Copyright 2024 Caleb Kronstad

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include "Core/Log.h"
#include "Core/FileUtility.h"
#include "Core/MathUtility.h"
#include "Core/TextUtility.h"
#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Window.h"

#include "Input/MouseCodes.h"
#include "Input/KeyCodes.h"
#include "Input/Event.h"
#include "Input/KeyEvent.h"
#include "Input/MouseEvent.h"

#include "UserInterface/ImGui/ImGuiLayer.h"

#include "PhysicsEngine/PhysicsSystem.h"
#include "PhysicsEngine/Animation/Bone.h"
#include "PhysicsEngine/Animation/Animation.h"
#include "PhysicsEngine/Animation/Animator.h"

#include "RenderingEngine/Shaders/Shader.h"
#include "RenderingEngine/Shaders/ShaderClasses.h"
#include "RenderingEngine/Camera.h"
#include "RenderingEngine/Lighting.h"
#include "RenderingEngine/Shadows.h"
#include "RenderingEngine/Transform.h"
#include "RenderingEngine/Mesh.h"
#include "RenderingEngine/Model.h"
#include "RenderingEngine/Components.h"
#include "RenderingEngine/Entity.h"
#include "RenderingEngine/Skybox.h"
#include "RenderingEngine/Scene.h"

#include "Core/Application.h"
#include "MainEngine/MainEngine.h"