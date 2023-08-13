#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Shader.h"
#include "engine/Camera.h"
#include "engine/LevelsSystem.h"
#include "engine/ui/editor/Icon.h"

class Renderer
{
public:

    Renderer(const Shader &shader);
    ~Renderer();

    void DrawIcon(Icon &iconData, Camera &cam, Texture &atlas);
    void DrawSprite(Level::Sprite &levelData, Camera &cam);

    static inline bool isWire = false;

private:

    void InitRenderData();

    Shader shader;
    GLuint quadVAO;
};
