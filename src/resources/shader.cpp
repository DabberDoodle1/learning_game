#include "resources/shader.hpp"

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

    int  success_status;
    char log_buffer[512]{};

    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success_status);
    if (!success_status) {
        glGetShaderInfoLog(GL_VERTEX_SHADER, 512, NULL, log_buffer);
        std::cerr << "Vertex shader error: " << log_buffer << std::endl;
    }

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success_status);
    if (!success_status) {
        glGetShaderInfoLog(GL_FRAGMENT_SHADER, 512, NULL, log_buffer);
        std::cerr << "Fragment shader error: " << log_buffer << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vert_shader);
    glAttachShader(ID, frag_shader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success_status);
    if (!success_status) {
        glGetProgramInfoLog(ID, 512, NULL, log_buffer);
        std::cerr << "Shader program error: " << log_buffer << std::endl;
    }

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
}

Shader::~Shader()
{
    if (ID == 0) {
        return;
    }

    glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::uniform(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::uniform(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::uniform(const char* name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, &value[0][0]);
}
