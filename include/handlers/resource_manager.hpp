#pragma once

#include "resources/drawable.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"

#include "imgui.h"

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

    // OpenGL resources
    static std::map<std::string, Shader>   shaders;
    static std::map<std::string, Texture>  textures;
    static std::map<std::string, Drawable> drawables;

    // Fonts
    static ImFont*      font_EN;
    static ImFont*      font_KR;
    static float        font_sizes[10];

    // Orthographic matrix for proper positioning and sizing
    static glm::mat4    projection;

    // Window data
    static GLFWwindow*  game_window;
    static unsigned int game_width;
    static unsigned int game_height;

    // Cursor data
    static double cursor_x;
    static double cursor_y;
};
