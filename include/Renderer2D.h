#ifndef SAL_RENDERER2D_H
#define SAL_RENDERER2D_H

#include "Basic.h"

struct Texture2D {
    u32 width;
    u32 height;
    u32 renderID;
};

namespace Renderer2D {

void Init();
void Shutdown();

Texture2D LoadTexture(const std::string& filename);

void Clear(glm::vec4 color);
void Flush();

void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color);
void DrawTexture(Texture2D texture, glm::vec2 position, glm::vec2 size, glm::vec4 color);

}

#endif