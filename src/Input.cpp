#include "engine/Input.h"
#include "engine/Renderer.h"
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
	if (mods == GLFW_MOD_SHIFT && action == GLFW_PRESS)
	{

	}

	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{

	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
	}

	//View wireframe
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		Renderer::isWire = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_L && action == GLFW_RELEASE)
	{
		Renderer::isWire = false;
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
	glfwGetFramebufferSize(window, &Config::FBW, &Config::FBH);
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
		glfwGetFramebufferSize(window, &Config::FBW, &Config::FBH);
	}
}


//Basic input
void Input::ProcessInput(GLFWwindow *window, Camera &cam, double deltaTime)
{
	auto CameraControlsUpdate = [&window, &cam, &deltaTime]()
	{
		static bool firstMouse = true;
		static double lastX{0.0}, lastY{0.0};
		bool disabledCursor = false;

		if (firstMouse)
		{
			glfwGetCursorPos(window, &lastX, &lastY);
			firstMouse = false;
		}

		double xpos{0.0}, ypos{0.0};
		glfwGetCursorPos(window, &xpos, &ypos);

		float xoffset = static_cast<float>(xpos - lastX);
		float yoffset = static_cast<float>(lastY - ypos);
		lastX = static_cast<double>(xpos);
		lastY = static_cast<double>(ypos);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
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
	};
	auto CameraUserExperience = [&window, &cam]()
	{
		static auto isAnimating = false;
		static auto startTime = 0.0;
		static auto start = glm::vec3(0.0F);
		static auto target = glm::vec3(0.0F, 0.0F, 5.0F);

		/*std::vector<glm::vec3> path =
		{
			glm::vec3(2.6F, 0.0F,  10.5F),
			glm::vec3(100.0F, 0.0F,  10.5F),
			glm::vec3(170.0F, 0.0F, -6.5F),
		};*/

		std::vector<glm::vec3> path =
		{
			glm::vec3(2.6F, 0.0F, 10.5F),
			glm::vec3(0.0F, -3.0F, -7.5F),
			glm::vec3(4.0F, 2.0F, 15.5F),
			glm::vec3(4.0F, 2.0F, 17.5F),
			glm::vec3(0.0F, 6.0F, -39.5F),
			glm::vec3(0.0F, 0.0F, -38.5F),
			glm::vec3(0.0F, 0.0F, -7.5F),
		};

		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && !isAnimating)
		{
			isAnimating = true;
			start = cam.getPosition();
			startTime = Time::FixedTime();
		}
		if (isAnimating && cam.AnimatePath(path, 15.0, startTime))
		{
			isAnimating = false;
		}


		//Change camera speed 
		static bool isShiftPressed = false;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			if (!isShiftPressed)
			{
				auto shifted = 4.0F;
				cam.setSpeed(shifted);
				isShiftPressed = true;
			}
		}
		else
		{
			if (isShiftPressed)
			{
				auto normal = 0.95F;
				cam.setSpeed(normal);
				isShiftPressed = false;
			}
		}
	};

	if (cam.isActiveInput())
		CameraControlsUpdate();
	CameraUserExperience();
}

