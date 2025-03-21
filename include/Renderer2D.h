#ifndef SAL_RENDERER2D_H
#define SAL_RENDERER2D_H

#include "Math.h"

struct Color
{
    float r;
    float g;
    float b;
    float a;
};

inline constexpr Color WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
inline constexpr Color BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
inline constexpr Color RED   = { 1.0f, 0.0f, 0.0f, 1.0f };
inline constexpr Color GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
inline constexpr Color BLUE  = { 0.0f, 0.0f, 1.0f, 1.0f };

void InitRenderer(int width, int height);

void ClearBackground(Color color);

void DrawLine(float x0, float y0, float x1, float y1, Color color);
void DrawLine(Vector2 start, Vector2 end, Color color);

void DrawCircle(float x, float y, float radius, Color color);
void DrawCircle(Vector2 position, float radius, Color color);

void DrawRect(float x, float y, float w, float h, Color color);
void DrawRect(Rect rect, Color color);

void DrawRectLines(float x, float y, float w, float h, Color color);
void DrawRectLines(Rect rect, Color color);

#endif