#include "engine/shared/BufferObject.h"
#include "engine/Draw.h"


void Draw::Quad(glm::vec3 pos, glm::vec2 scale, Texture &texture)
{
    texture.bind();

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(pos.x, pos.y, pos.z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(pos.x + scale.x, pos.y, pos.z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(pos.x + scale.x, pos.y + scale.y, pos.z);

    glTexCoord2f(1.0f, 1.0f); glVertex3f(pos.x + scale.x, pos.y + scale.y, pos.z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(pos.x, pos.y + scale.y, pos.z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(pos.x, pos.y, pos.z);
    glEnd();

    texture.unbind();
}

