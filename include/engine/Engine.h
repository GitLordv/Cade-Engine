#pragma once

#include "engine/Renderer.h"
#include "engine/Texture.h"
#include "engine/Input.h"
#include "engine/LevelsSystem.h"
#include "engine/TriggerBox.h"
#include "engine/Camera.h"

#include "engine/shared/Utils.h"
#include "engine/shared/Config.h"
#include "engine/shared/Time.h"
#include "engine/shared/Shader.h"
#include "engine/shared/Config.h"
#include "engine/shared/UVRect.h"
#include "engine/shared/GLCallbacks.h"

#include "engine/audio/AudioSystem.h"


class Engine
{
public:
	Engine() = delete;
	~Engine() = delete;

	static void Startup();
	static void Render();

private:
	static void Init();
	static void Destroy();
	static void RawInput(GLFWwindow* window, Camera &cam);
	static void CallbacksInput(GLFWwindow* window);

	static inline GLFWwindow* editorWindow;
};


