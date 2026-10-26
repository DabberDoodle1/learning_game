#include "handlers/resource_manager.hpp"

#include <glm/ext/matrix_clip_space.hpp>

std::map<std::string, Shader>   ResourceManager::shaders;
std::map<std::string, Texture>  ResourceManager::textures;
std::map<std::string, Drawable> ResourceManager::drawables;

ImFont*      ResourceManager::font_EN;
ImFont*      ResourceManager::font_KR;
float        ResourceManager::font_sizes[10];

glm::mat4    ResourceManager::projection;

GLFWwindow*  ResourceManager::game_window;
int          ResourceManager::game_width;
int          ResourceManager::game_height;

double ResourceManager::cursor_x;
double ResourceManager::cursor_y;

float ResourceManager::scaling_factor;

std::unique_ptr<Scene> ResourceManager::active_scene;

void ResourceManager::clear()
{
    shaders.clear();
    textures.clear();
    drawables.clear();
}
