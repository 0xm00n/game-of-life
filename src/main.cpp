#include "simulation/Simulator.h"
#include "rendering/Window.h"
#include "rendering/Shader.h"
#include "rendering/Renderer.h"
#include "rendering/Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <algorithm>

const char* grid_vertex_source = R"(
#version 460 core
layout (location = 0) in vec2 aPos;
uniform mat4 uInverseProjection;
out vec2 worldPos;

void main() {
    vec4 world = uInverseProjection * vec4(aPos, 0.0, 1.0);
    worldPos = world.xy;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* grid_fragment_source = R"(
#version 460 core
in vec2 worldPos;
out vec4 fragColor;

void main() {
    vec2 grid = abs(fract(worldPos) - 0.5);
    float line = min(grid.x, grid.y);

    if (line < 0.03) {
        fragColor = vec4(0.15, 0.15, 0.15, 1.0);
    } else {
        discard; 
    }
}
)";

const char* vertex_source = R"(
#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset;  // instance position
uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * vec4(aPos + aOffset, 0.0, 1.0);
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
    Shader grid_shader(grid_vertex_source, grid_fragment_source);
    Shader shader(vertex_source, fragment_source);
    Renderer renderer;
    Camera cam(800.0f / 600.0f);

    Simulator sim(100, 100);

    // glider
    sim.set_cell(1, 0, true);
    sim.set_cell(2, 1, true);
    sim.set_cell(0, 2, true);
    sim.set_cell(1, 2, true);
    sim.set_cell(2, 2, true);

    GLFWwindow* handle = win.get_handle();

    bool paused = true;
    bool space_was_pressed = false;

    int frame_cnt = 0;
    int steps_per_sec = 2;

    int generation = 0;

    while (win.is_open()) {
        std::string title = "Game of Life - Generation: " + std::to_string(generation) +
                            " | Speed: " + std::to_string(steps_per_sec) + "/s" +
                            (paused ? " [PAUSED]" : "");
        glfwSetWindowTitle(handle, title.c_str());

        int width, height;
        glfwGetFramebufferSize(win.get_handle(), &width, &height);
        glViewport(0, 0, width, height);
        cam.set_aspect_ratio((float)width / height);

        // pan with wasd
        float pan_speed = 0.01f * cam.get_zoom();
        if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS) cam.pan(0, pan_speed);
        if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS) cam.pan(0, -pan_speed);
        if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS) cam.pan(-pan_speed, 0);
        if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS) cam.pan(pan_speed, 0);

        // zoom with q and e
        if (glfwGetKey(handle, GLFW_KEY_Q) == GLFW_PRESS) cam.adjust_zoom(1.02f);
        if (glfwGetKey(handle, GLFW_KEY_E) == GLFW_PRESS) cam.adjust_zoom(0.98f);

        bool space_pressed = glfwGetKey(handle, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (space_pressed && !space_was_pressed) {
            paused = !paused;
        }
        space_was_pressed = space_pressed;

        static bool up_was_pressed = false;
        bool up_pressed = glfwGetKey(handle, GLFW_KEY_UP) == GLFW_PRESS;
        if (up_pressed && !up_was_pressed) {
            steps_per_sec = std::min(steps_per_sec + 1, 60);
        }
        up_was_pressed = up_pressed;

        static bool down_was_pressed = false;
        bool down_pressed = glfwGetKey(handle, GLFW_KEY_DOWN) == GLFW_PRESS;
        if (down_pressed && !down_was_pressed) {
            steps_per_sec = std::max(steps_per_sec - 1, 1);
        }
        down_was_pressed = down_pressed;

        // step once with right arrow
        static bool right_was_pressed = false;
        bool right_pressed = glfwGetKey(handle, GLFW_KEY_RIGHT) == GLFW_PRESS;
        if (right_pressed &&  !right_was_pressed) {
            sim.step();
            generation++;
        }
        right_was_pressed = right_pressed;

        if (!paused) {
            frame_cnt++;
            if (frame_cnt >= 60 / steps_per_sec) {
                sim.step();
                generation++;
                frame_cnt = 0;
            }
        }

        // collect live cell positions
        std::vector<float> positions;
        const Grid& grid = sim.get_grid();
        for (int y = 0; y < grid.get_height(); y++) {
            for (int x = 0; x < grid.get_width(); x++) {
                if (grid.is_alive(x, y)) {
                    positions.push_back((float)x);
                    positions.push_back((float)y);
                }
            }
        }

        renderer.update_cells(positions);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 inv_proj = glm::inverse(cam.get_matrix());
        grid_shader.use();
        grid_shader.set_mat4("uInverseProjection", glm::value_ptr(inv_proj));
        renderer.draw_grid();

        shader.use();
        shader.set_mat4("uProjection", glm::value_ptr(cam.get_matrix()));
        renderer.draw();

        win.update();
    }

    return 0;
}
