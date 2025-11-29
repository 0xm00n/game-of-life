#pragma once
#include <GL/glew.h>
#include <vector>

class Renderer {
private: 
    GLuint vao;             // vertex array object: stores vertex attribute config
    GLuint vbo;             // vertex buffer object: stores vertex data, quad vertices (static)
    GLuint instance_vbo;    // cell positions updated each frame
    int instance_cnt;

    GLuint grid_vao;
    GLuint grid_vbo;

public:
    Renderer();
    ~Renderer();

    void update_cells(const std::vector<float>& positions); // upload cell positions
    void draw();
    void draw_grid();

};