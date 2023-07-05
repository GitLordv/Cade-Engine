#include "engine/shared/Config.h"
#include "engine/shared/Time.h"
#include "engine/Camera.h"
#include "engine/Engine.h"
#include "engine/Input.h"


extern GLFWwindow *window;


//Callbacks
void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{

}

void Input::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{

}


void Input::CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{

}


void Input::KeyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		std::cout << "\nLoading config.." << std::endl;
		Config::LoadConfig(window);
	}

	//View wireframe
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		glBlendFunc(GL_ZERO, GL_ZERO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


void Input::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(window, &Config::fbW, &Config::fbH);
}


void Input::WindowSizeCallback(GLFWwindow *window, int width, int height)
{

}


void Input::WindowCloseCallback(GLFWwindow *window)
{
	Config::SaveConfig(window);
}


//Raw
void Input::ProcessInput(GLFWwindow *window, Camera &cam, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{

	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
	}

	//Camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.KeyboardInput(CameraDirection::FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.KeyboardInput(CameraDirection::BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.KeyboardInput(CameraDirection::LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.KeyboardInput(CameraDirection::RIGHT, deltaTime);
	}


	// Обработка мыши
	static bool firstMouse = true;
	static double lastX = 0, lastY = 0;
	bool disabledCursor = false;

	if (firstMouse)
	{
		glfwGetCursorPos(window, &lastX, &lastY);
		firstMouse = false;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = (float)xpos;
	lastY = (float)ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		disabledCursor = true;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		disabledCursor = false;
	}


	if (disabledCursor)
	{
		cam.MouseMovingInput(xoffset, yoffset, true);
	}
}
