#include "rendering/Shader.h"
#include <stdexcept>

GLuint Shader::compile(GLenum type, const char* source) {
    // create empty shader of given type
    GLuint shader = glCreateShader(type);

    // attach source code to shader
    glShaderSource(shader, 1, &source, nullptr);

    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    return shader;
}

Shader::Shader(const char* vertex_source, const char* fragment_source) {
    GLuint vertex_shader = compile(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_source);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::set_mat4(const char* name, const float* matrix) {
    GLint loc = glGetUniformLocation(program, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}