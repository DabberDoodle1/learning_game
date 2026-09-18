#include "resource_manager.hpp"

// OpenGL resources
std::map<std::string, Shader>   ResourceManager::shaders;
std::map<std::string, Drawable> ResourceManager::drawables;

// Fonts
ImFont*      ResourceManager::font_EN;
ImFont*      ResourceManager::font_KR;
float        ResourceManager::font_sizes[10];

// Matrices
glm::mat4    ResourceManager::view;
glm::mat4    ResourceManager::projection;

// Game
GLFWwindow*  ResourceManager::game_window;
unsigned int ResourceManager::game_width;
unsigned int ResourceManager::game_height;

void ResourceManager::clear()
{
    shaders.clear();
    drawables.clear();
}
