#include "rendering/Renderer.h"

// fullscreen quad for grid in clip space -1 to 1
float grid_vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f, -1.0f,
    1.0f,  1.0f,
    -1.0f,  1.0f,
};

// unit quad centered at origin
float vertices[] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f,  0.5f,
    0.5f, -0.5f,
    0.5f,  0.5f,
    -0.5f,  0.5f,
};

Renderer::Renderer() : instance_cnt(0) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &instance_vbo);

    glBindVertexArray(vao);

    // quad vertices, attribute 0
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // instance positions, attribute 1
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);    // makes it per instance, not per vertex

    // grid
    glGenVertexArrays(1, &grid_vao);
    glGenBuffers(1, &grid_vbo);
    glBindVertexArray(grid_vao);
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind VAO
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &instance_vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &grid_vbo);
    glDeleteVertexArrays(1, &grid_vao);

}

void Renderer::update_cells(const std::vector<float>& positions) {
    instance_cnt = positions.size() / 2;    // 2 floats per position

    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);
}

void Renderer::draw() {
    if (instance_cnt == 0) return;

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instance_cnt);
}

void Renderer::draw_grid() {
    glBindVertexArray(grid_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}