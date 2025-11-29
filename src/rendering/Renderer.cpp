#include "rendering/Renderer.h"

// unit quad centered at origin
float vertices[] = {
    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f,  0.5f,
    0.5f, -0.5f,
    0.5f,  0.5f,
    -0.5f,  0.5f,
};

Renderer::Renderer() {
    // generate VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind VAO and VBO
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind VAO
    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Renderer::draw() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}