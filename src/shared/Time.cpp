#include "engine/shared/Time.h"


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
	static double deltaTime, oldTime, newTime;

	newTime = mainTime;

	deltaTime = newTime - oldTime;
	oldTime = newTime;

	return deltaTime;
}