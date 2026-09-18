#pragma once

#include "drawable.hpp"
#include "imgui.h"
#include "shader.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <map>
#include <string>

struct ResourceManager {
    ResourceManager()                                        = delete;
    ResourceManager(const ResourceManager& other)            = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;

    static void clear();

    static std::map<std::string, Shader>   shaders;
    static std::map<std::string, Drawable> drawables;

    // Fonts
    static ImFont*      font_EN;
    static ImFont*      font_KR;
    static float        font_sizes[10];

    static glm::mat4    view;
    static glm::mat4    projection;

    static GLFWwindow*  game_window;
    static unsigned int game_width;
    static unsigned int game_height;
};
