#include "engine/shared/Config.h"
#include "engine/shared/Time.h"
#include "engine/Camera.h"
#include "engine/Engine.h"
#include "engine/Input.h"

#include "engine/audio/AudioSystem.h"


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
	static ALuint Music1, Music2;

	static bool isLoaded = false;
	if (mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
	{
		/*if (!isLoaded)
		{
			Music1 = AudioSystem::LoadSound("Data/Music/Jibberish Jungle - Danger Ahead_cut.wav");
			Music2 = AudioSystem::LoadSound("Data/Music/The Tower of Babel (Rising).wav");
			isLoaded = true;
		}*/
	}

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{

	}

	static bool isPressed = false;
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (!isPressed && isLoaded)
		{
			AudioSystem::StopSound(Music2);
			AudioSystem::PlaySound(Music1, 0.6f, true);
			//AudioSystem::PauseSound(1);
			isPressed = true;
		}
		else if (isPressed && isLoaded)
		{
			//AudioSystem::UnPauseSound(1);
			AudioSystem::StopSound(Music1);
			AudioSystem::PlaySound(Music2, 0.6f, true);
			isPressed = false;
		}
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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
		Config::SaveConfig(window);
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


void Input::WindowMaximizeCallback(GLFWwindow *window, int maximized)
{
	if (maximized)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glfwGetFramebufferSize(window, &Config::fbW, &Config::fbH);
	}
}

float aTwe = 0.0f;
//Raw
void Input::ProcessInput(GLFWwindow *window, Camera &cam, double deltaTime)
{
	// Обработка мыши
	static bool firstMouse = true;
	static double lastX{0.0}, lastY{0.0};
	bool disabledCursor = false;

	if (firstMouse)
	{
		glfwGetCursorPos(window, &lastX, &lastY);
		firstMouse = false;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos); // reversed since y-coordinates go from bottom to top
	lastX = static_cast<double>(xpos);
	lastY = static_cast<double>(ypos);

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

	//Moving logic
	if (disabledCursor)
	{
		cam.MouseMovingInput(xoffset, yoffset);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			cam.KeyboardInput(CameraDirection::UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			cam.KeyboardInput(CameraDirection::DOWN, deltaTime);
		}
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

	}


	static auto isAnimating = false;
	static auto startTime = 0.0;
	static auto start = glm::vec3(0.0f);
	static auto target = glm::vec3(-10.0f, 60.0f, 100.0f);
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !isAnimating)
	{
		isAnimating = true;
		start = cam.getPosition();
		startTime = Time::GetMainTime();

	}
	if (isAnimating && cam.AnimatePos(start, target, 1.5, startTime))
	{
		isAnimating = false;
	}
}
