#include "resource_manager.hpp"

std::map<std::string, Shader>   ResourceManager::shaders;
std::map<std::string, Drawable> ResourceManager::drawables;

void ResourceManager::clear()
{
    shaders.clear();
    drawables.clear();
}
