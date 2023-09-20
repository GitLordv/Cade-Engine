#pragma once

#include "engine/shared/Utils.h"
#include "engine/shared/Shader.h"
#include "engine/Texture.h"
#include "engine/Camera.h"
#include "engine/Level.h"
#include "engine/ui/editor/Icon.h"
#include "engine/shared/AABB.h"

class Renderer
{
public:

    Renderer(const Shader &shader);
    ~Renderer();

    void DrawIcon(IconData &data, Camera &cam, Texture &atlas);
    void DrawSprite(LevelData &levelData, Camera &cam);
    void DrawAABB(const AABB &aabb);
    void DrawStrip(glm::vec3 position, Texture &texture, std::vector<GLfloat> points);
    //void DrawStrip(glm::vec3 position, Texture &texture, std::vector<GLfloat> points, std::vector<GLfloat> indices);

    static inline bool isWire = false;

private:

    void InitRenderData();

    Shader shader;
    GLuint quadVAO;
};
