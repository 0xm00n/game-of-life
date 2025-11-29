#include "rendering/Window.h"
#include <stdexcept>

Window::Window(int width, int height, const char* title)
    : width(width), height(height), win(nullptr) {

    if (!glfwInit()) {
        throw std::runtime_error("Failed to init GLFW");
    }

    // request OpenGL 4.6 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win  =glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    // make this window's OpenGL context active
    glfwMakeContextCurrent(win);
    glewInit();
}

Window::~Window() {
    if (win) {
        glfwDestroyWindow(win);
    }
    glfwTerminate();
}

bool Window::is_open() const {
    return !glfwWindowShouldClose(win);
}

void Window::update() {
    glfwSwapBuffers(win);   // show what we rendered
    glfwPollEvents();       // handle keyboard + mouse + close events
}
