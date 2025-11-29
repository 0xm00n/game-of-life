#include "rendering/Window.h"
#include "rendering/Shader.h"
#include "rendering/Renderer.h"

const char* vertex_source = R"(
#version 460 core
layout (location = 0) in vec2 aPos;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragment_source = R"(
#version 460 core
out vec4 frag_color;

void main() {
    frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}
)";

int main() {
    Window win(800, 600, "Game of Life");
    Shader shader(vertex_source, fragment_source);
    Renderer renderer;

    while (win.is_open()) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        renderer.draw();

        win.update();
    }

    return 0;
}
