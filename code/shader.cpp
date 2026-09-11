#include "shader.hpp"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

void get_shader_code(const char* file_path, std::string& code_buffer)
{
    std::ifstream     fs;
    std::stringstream ss;

    fs.open(file_path);

    if (!fs.is_open()) {
        std::cerr << "File at \"" << file_path << "\" not found." << std::endl;
        return;
    }

    ss << fs.rdbuf();
    fs.close();

    code_buffer = ss.str();
}

Shader::Shader(const char* vert_path, const char* frag_path)
{
    // Shader code buffers
    std::string vert_buffer;
    std::string frag_buffer;
    const char* vert_c_str;
    const char* frag_c_str;

    // Get shader code and store in buffers
    get_shader_code(vert_path, vert_buffer);
    get_shader_code(frag_path, frag_buffer);
    vert_c_str = vert_buffer.c_str();
    frag_c_str = frag_buffer.c_str();

    // Create shader program
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vert_shader, 1, &vert_c_str, NULL);
    glShaderSource(frag_shader, 1, &frag_c_str, NULL);
    glCompileShader(vert_shader);
    glCompileShader(frag_shader);

    ID = glCreateProgram();
    glAttachShader(ID, vert_shader);
    glAttachShader(ID, frag_shader);
    glLinkProgram(ID);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}
