#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Game {
public:
    static void setup(unsigned int width, unsigned int height, const char* title);
    static void run();
    static void finish();

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
