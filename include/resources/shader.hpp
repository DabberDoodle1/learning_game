#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float4.hpp>

class Shader {
public:
    Shader(const char* vert_path, const char* frag_path);
    ~Shader();

    void use() const;
    void uniform(const char* name, const float value) const;
    void uniform(const char* name, const int value) const;
    void uniform(const char* name, const glm::mat4& value) const;
    void uniform(const char* name, const glm::vec4& value) const;

private:
    unsigned int ID;
};
