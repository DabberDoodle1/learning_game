#pragma once

class Texture {
public:
    Texture(const char* texture_path);
    ~Texture();

    void bind() const;

private:
    unsigned int ID;
};
