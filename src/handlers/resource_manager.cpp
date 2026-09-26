#include "handlers/resource_manager.hpp"

#include <glm/ext/matrix_clip_space.hpp>

// OpenGL resources
std::map<std::string, Shader>   ResourceManager::shaders;
std::map<std::string, Texture>  ResourceManager::textures;
std::map<std::string, Drawable> ResourceManager::drawables;

// Fonts
ImFont*      ResourceManager::font_EN;
ImFont*      ResourceManager::font_KR;
float        ResourceManager::font_sizes[10];

// Universal rendering matrices
glm::mat4    ResourceManager::projection;

// Window data
GLFWwindow*  ResourceManager::game_window;
unsigned int ResourceManager::game_width;
unsigned int ResourceManager::game_height;

double ResourceManager::cursor_x;
double ResourceManager::cursor_y;

void ResourceManager::clear()
{
    shaders.clear();
    textures.clear();
    drawables.clear();
}
