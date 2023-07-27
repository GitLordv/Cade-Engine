#pragma once

#include "engine/shared/Utils.h"


class Input
{
public:
	//Callbacks
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void WindowSizeCallback(GLFWwindow* window, int width, int height);
	static void WindowCloseCallback(GLFWwindow* window);
	static void WindowMaximizeCallback(GLFWwindow* window, int maximized);

	//Raw
	static void ProcessInput(GLFWwindow* window, Camera& cam, double deltaTime);

private:

};




