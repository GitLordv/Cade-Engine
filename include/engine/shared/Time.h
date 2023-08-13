#pragma once

class Time
{
public:
	static void Start();
	static void Restart();
	static double FixedTime();
	static double DeltaTime();

private:
	static inline double mainTime;
};





