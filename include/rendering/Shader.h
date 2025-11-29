#pragma once
#include <GL/glew.h>
#include <string>

class Shader {
private:
    GLuint program;

    // helper: compile a single shader, return its ID
    GLuint compile(GLenum type, const char* source);

public:
    Shader(const char* vertex_source, const char* fragment_source);
    ~Shader();

    void use();

    // set uniforms
    void set_mat4(const char* name, const float* matrix);
};