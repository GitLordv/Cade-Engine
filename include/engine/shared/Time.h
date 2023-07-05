#pragma once

#include "engine/shared/Utils.h"


class Time
{
public:
	static void Start();
	static void Restart();
	static double GetMainTime();
	static double GetDeltaTime();

private:
	static inline double mainTime;
};





