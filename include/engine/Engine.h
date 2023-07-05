#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Config.h"
#include "engine/Camera.h"


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
	static void RawInput(GLFWwindow* window);
	static void CallbacksInput(GLFWwindow* window);
};


