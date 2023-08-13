#include "engine/Engine.h"
#include "engine/shared/Config.h"

#include <fstream>
#include <tuple>


void Config::SaveConfig(GLFWwindow *window)
{
	std::ofstream fout(dataFolder + "/" + configName);

	//Window
	int szW, szH;
	int fbW, fbH;
	glfwGetWindowSize(window, &szW, &szH);
	glfwGetFramebufferSize(window, &fbW, &fbH);

	std::pair<int, int> WindowSize{szW, szH};
	config["windowWidth"] = std::get<0>(WindowSize);
	config["windowHeight"] = std::get<1>(WindowSize);

	std::pair<int, int> FramebufferSize{fbW, fbH};
	config["framebufferWidth"] = std::get<0>(FramebufferSize);
	config["framebufferHeight"] = std::get<1>(FramebufferSize);

	fout << config;
}



void Config::LoadConfig(GLFWwindow *window)
{
	try
	{
		config = YAML::LoadFile(dataFolder + "/" + configName);

		//Load window size
		const auto paramW = config["windowWidth"].as<std::string>();
		const auto paramH = config["windowHeight"].as<std::string>();
		auto winWidth = std::stoi(paramW);
		auto winHeight = std::stoi(paramH);
		glfwSetWindowSize(window, winWidth, winHeight);

		//Load framebuffer size
		const auto paramfbW = config["framebufferWidth"].as<std::string>();
		const auto paramfbH = config["framebufferHeight"].as<std::string>();
		auto fbWidth = std::stoi(paramfbW);
		auto fbHeight = std::stoi(paramfbH);
		glViewport(0, 0, fbWidth, fbHeight);
	}
	catch (...)
	{
		std::clog << "Log: Configure file not loaded\n";
		std::clog << "Config name: " << configName << std::endl;
	}
}



