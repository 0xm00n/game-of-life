#pragma once
#include <GL/glew.h>

class Renderer {
private: 
    GLuint vao; // vertex array object: stores vertex attribute config
    GLuint vbo; // vertex buffer object: stores vertex data

public:
    Renderer();
    ~Renderer();

    void draw();

};