#pragma once

class Camera;
struct GLFWwindow;

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
	static void RawInput(GLFWwindow *window, Camera &cam);
	static void CallbacksInput(GLFWwindow *window);

	static inline GLFWwindow *editorWindow;
};

