#include "engine/shared/Time.h"
#include "GLFW/glfw3.h"

void Time::Start()
{
	mainTime = glfwGetTime();
}

void Time::Restart()
{
	glfwSetTime(0.0);
}

double Time::GetMainTime()
{
	return mainTime;
}

double Time::GetDeltaTime()
{
	static double oldTime, newTime;

	newTime = mainTime;

	auto deltaTime = newTime - oldTime;
	oldTime = newTime;

	return deltaTime;
}