workspace "NiftyEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Sandbox"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDir = {}
IncludeDir["GLAD"] = os.realpath("Vendor/glad/include")
IncludeDir["GLFW"] = os.realpath("Vendor/glfw/include")
IncludeDir["IMGUI"] = os.realpath("Vendor/imgui")
IncludeDir["STB_IMAGE"] = os.realpath("Vendor/stb_image")
IncludeDir["ASSIMP"] = os.realpath("Vendor/assimp/include")
IncludeDir["GLM"] = os.realpath("Vendor/glm/glm")
IncludeDir["IMGUIZMO"] = os.realpath("Vendor/imguizmo")

group "Other"
   include "Vendor/glad/Build-GLAD.lua"
   include "Vendor/glfw/Build-GLFW.lua"
   include "Vendor/imgui/Build-IMGUI.lua"
   include "Vendor/assimp/Build-ASSIMP.lua"
   include "Vendor/glm/Build-GLM.lua"
   include "Vendor/imguizmo/Build-IMGUIZMO.lua"
group ""

include "Nifty/Build-Core.lua"

include "Sandbox/Build-App.lua"