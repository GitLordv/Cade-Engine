#include "engine/Engine.h"
#include "engine/shared/Config.h"

#include <fstream>
#include <tuple>


void Config::SaveConfig(GLFWwindow *window)
{
	std::ofstream fout(dataFolder + "/" + configName);

	//Window
	int sizeW, sizeH;
	glfwGetWindowSize(window, &sizeW, &sizeH);

	std::pair<int, int> size{sizeW, sizeH};
	config["windowWidth"] = std::get<0>(size);
	config["windowHeight"] = std::get<1>(size);

	fout << config;
}



void Config::LoadConfig(GLFWwindow *window)
{
	try
	{
		config = YAML::LoadFile(dataFolder + "/" + configName);

		//Window size parse params
		const auto paramW = config["windowWidth"].as<std::string>();
		const auto paramH = config["windowHeight"].as<std::string>();

		auto winWidth = std::stoi(paramW);
		auto winHeight = std::stoi(paramH);

		glfwSetWindowSize(window, winWidth, winHeight);

	}
	catch (...)
	{
		std::clog << "Log: Configure file not loaded.\n";
	}
}



