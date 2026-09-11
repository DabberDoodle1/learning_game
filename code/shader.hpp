#pragma once

class Shader {
public:
    Shader(const char* vert_path, const char* frag_path);
    ~Shader();

    Shader(const Shader& other)            = delete;
    Shader(Shader&& other)                 = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other)      = delete;

    void use() const;

private:
    unsigned int ID;
};
