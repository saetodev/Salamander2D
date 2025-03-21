#ifndef SAL_WINDOW_H
#define SAL_WINDOW_H

void InitWindow(int width, int height, const char* title);
void ShutdownWindow();

void SwapWindow();
bool WindowClosed();



#endif