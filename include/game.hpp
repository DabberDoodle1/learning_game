#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Game {
    void setup(unsigned int width, unsigned int height, const char* title);
    void run();
    void finish();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursor_enter_callback(GLFWwindow* window, int entered);
    void cursor_pos_callback(GLFWwindow* window, double _x, double _y);
};
