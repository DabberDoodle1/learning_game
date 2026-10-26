#pragma once

#include "resources/drawable.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "scenes/scene.hpp"

#include "imgui.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_float4x4.hpp>

#include <map>
#include <memory>
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

    // Universal orthographic projection matrix
    static glm::mat4    projection;

    // Window
    static GLFWwindow*  game_window;
    static int          game_width;
    static int          game_height;

    // Cursor
    static double cursor_x;
    static double cursor_y;

    // Scaling factor modifier - magic number sizes are intended for 1280x720 resolution and this is for scaling across differnet resolutions.
    static float scaling_factor;

    // Scene
    static std::unique_ptr<Scene> active_scene;
};
