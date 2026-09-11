#pragma once
#include "drawable.hpp"
#include "shader.hpp"

#include <map>
#include <string>

struct ResourceManager {
    ResourceManager()                                        = delete;
    ResourceManager(const ResourceManager& other)            = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;

    static void clear();

    static std::map<std::string, Shader>   shaders;
    static std::map<std::string, Drawable> drawables;
};
