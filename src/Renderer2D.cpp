#include "Renderer2D.h"

#include <cmath>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void InitRenderer(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
}

void ClearBackground(Color color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DrawLine(float x0, float y0, float x1, float y1, Color color)
{
    DrawLine({ x0, y0 }, { x1, y1 }, color);
}

void DrawLine(Vector2 start, Vector2 end, Color color)
{
    glBegin(GL_LINES);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(start.x, start.y);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(end.x, end.y);

    glEnd();
}

void DrawCircle(float x, float y, float radius, Color color)
{
    DrawCircle({ x, y }, radius, color);
}

void DrawCircle(Vector2 position, float radius, Color color)
{
    static int n = 32;

    float angle = 0.0f;
    float step  = 2 * PI / n;

    glBegin(GL_POLYGON);

    for (int i = 0; i < n; i++)
    {
        float x = cos(angle);
        float y = sin(angle);

        glColor4f(color.r, color.g, color.b, color.a);
        glVertex2f(position.x + radius * x, position.y + radius * y);

        angle += step;
    }

    glEnd();
}

void DrawRect(float x, float y, float w, float h, Color color)
{
    DrawRect({ x, y, w, h }, color);
}

void DrawRect(Rect rect, Color color)
{
    glBegin(GL_QUADS);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x, rect.y);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x + rect.w, rect.y);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x + rect.w, rect.y + rect.h);

    glColor4f(color.r, color.g, color.b, color.a);
    glVertex2f(rect.x, rect.y + rect.w);
    
    glEnd();
}

void DrawRectLines(float x, float y, float w, float h, Color color)
{
    DrawRectLines({ x, y, w, h }, color);
}

void DrawRectLines(Rect rect, Color color)
{
    float x0 = rect.x;
    float y0 = rect.y;
    float x1 = rect.x + rect.w;
    float y1 = rect.y + rect.h;

    DrawLine(x0, y0, x1, y0, color);
    DrawLine(x1, y0, x1, y1, color);
    DrawLine(x0, y1, x1, y1, color);
    DrawLine(x0, y0, x0, y1, color);
}