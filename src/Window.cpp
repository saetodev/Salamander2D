#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct InputButton {
    bool down;
    bool pressed;
    bool released;
};

static GLFWwindow* s_window;

static f32 s_lastTime;
static f32 s_deltaTime;

static std::array<InputButton, NUM_KEY_CODES> s_keyboard;
static std::array<InputButton, NUM_MOUSE_BUTTONS> s_mouse;

static glm::vec2 s_mousePosition;

static void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= s_keyboard.size()) {
        return;
    }

    bool isDown  = (action == GLFW_PRESS)   || (action == GLFW_REPEAT);
    bool wasDown = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);

    s_keyboard[key].down     = isDown;
    s_keyboard[key].pressed  = isDown && !wasDown;
    s_keyboard[key].released = !isDown && wasDown;
}

static void WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button < 0 || button >= s_mouse.size()) {
        return;
    }

    bool isDown  = (action == GLFW_PRESS)   || (action == GLFW_REPEAT);
    bool wasDown = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);

    s_mouse[button].down     = isDown;
    s_mouse[button].pressed  = isDown && !wasDown;
    s_mouse[button].released = !isDown && wasDown;
}

static void WindowMousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    s_mousePosition = { xpos, ypos };
}

void Window::Init(int width, int height, const std::string& title) {
    if (!glfwInit()) {
        const char* message = NULL;
        glfwGetError(&message);

        std::cout << "ERROR: " << message << std::endl;
        std::exit(-1);
    }

    s_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    
    if (!s_window) {
        const char* message = NULL;
        glfwGetError(&message);

        std::cout << "ERROR: " << message << std::endl;
        std::exit(-1);
    }

    glfwSetKeyCallback(s_window, WindowKeyCallback);
    glfwSetMouseButtonCallback(s_window, WindowMouseButtonCallback);
    glfwSetCursorPosCallback(s_window, WindowMousePosCallback);

    glfwMakeContextCurrent(s_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Failed to load OpenGL" << std::endl;
        std::exit(-1);
    }
}

void Window::Shutdown() {
    glfwDestroyWindow(s_window);
    glfwTerminate();
}

void Window::SwapBuffers() {
    for (InputButton& key : s_keyboard) {
        key.pressed  = false;
        key.released = false; 
    }

    for (InputButton& button : s_mouse) {
        button.pressed  = false;
        button.released = false;
    }

    glfwSwapBuffers(s_window);
    glfwPollEvents();

    f32 nowTime = glfwGetTime();
    s_deltaTime = nowTime - s_lastTime;
    s_lastTime  = nowTime;
}

bool Window::Closed() {
    return glfwWindowShouldClose(s_window);
}

f32 Window::FrameTime() {
    return s_deltaTime;
}

bool Input::KeyDown(KeyCode key) {
    return s_keyboard[key].down;
}

bool Input::KeyPressed(KeyCode key) {
    return s_keyboard[key].pressed;
}

bool Input::KeyReleased(KeyCode key) {
    return s_keyboard[key].released;
}

bool Input::MouseDown(MouseButton button) {
    return s_mouse[button].down;
}

bool Input::MousePressed(MouseButton button) {
    return s_mouse[button].pressed;
}

bool Input::MouseReleased(MouseButton button) {
    return s_mouse[button].released;
}

glm::vec2 Input::MousePosition() {
    return s_mousePosition;
}