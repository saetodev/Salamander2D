#include "Window.h"

#include <iostream>

#include <GLFW/glfw3.h>

static GLFWwindow* s_window;

void InitWindow(int width, int height, const char* title)
{
    if (!glfwInit())
    {
        const char* message = NULL;
        glfwGetError(&message);

        std::cout << "[ERROR]: " << message << std::endl;
        std::exit(-1);
    }

    s_window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!s_window)
    {
        const char* message = NULL;
        glfwGetError(&message);

        std::cout << "[ERROR]: " << message << std::endl;
        std::exit(-1);
    }

    glfwMakeContextCurrent(s_window);
}

void ShutdownWindow()
{
    glfwDestroyWindow(s_window);
    glfwTerminate();
}

void SwapWindow()
{
    glfwSwapBuffers(s_window);
    glfwPollEvents();
}

bool WindowClosed()
{
    return glfwWindowShouldClose(s_window);
}