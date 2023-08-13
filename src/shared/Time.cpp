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

double Time::FixedTime()
{
	return mainTime;
}

double Time::DeltaTime()
{
    static double oldTime = mainTime; // Запоминаем предыдущее значение mainTime.
    double newTime = glfwGetTime();

    double deltaTime = newTime - oldTime;
    oldTime = newTime;

    return deltaTime;
}