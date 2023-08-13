#pragma once

#include "engine/shared/Utils.h"
#include "yaml-cpp/yaml.h"


class Config
{
public:

	static void SaveConfig(GLFWwindow *window);
	static void LoadConfig(GLFWwindow *window);

	static inline unsigned int winW = 900;
	static inline unsigned int winH = 800;

	static inline int FBW = winW;
	static inline int FBH = winH;

private:

	static inline YAML::Node config;
	static inline const std::string &configName = "EngineConfig.yml";

};


