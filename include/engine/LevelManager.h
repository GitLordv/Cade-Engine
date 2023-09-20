#pragma once 

#include "engine/Level.h"

class LevelManager
{
public:
    LevelManager(Camera &cam);

    void SwitchLevel(std::string_view path);

private:

    Level currentLevel;
    Camera &cam;
};