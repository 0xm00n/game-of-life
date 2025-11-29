#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
private:
    GLFWwindow* win;
    int width;
    int height;

public: 
    Window(int width, int height, const char* title);
    ~Window();

    bool is_open() const;
    void update();

    GLFWwindow* get_handle() const { return win; }
    int get_width() const { return width; }
    int get_height() const { return height; }
};