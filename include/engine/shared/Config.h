#pragma once

#include "engine/shared/Utils.h"
#include "yaml-cpp/yaml.h"


class Config
{
public:

	static void SaveConfig(GLFWwindow *window);
	static void LoadConfig(GLFWwindow *window);

	static inline uint windowWidth  = 1600;
	static inline uint windowHeight = 900;

	static inline int fbW = NULL;
	static inline int fbH = NULL;


private:

	static inline YAML::Node config;
	static inline const std::string &configName = "EngineConfig.yml";

};


