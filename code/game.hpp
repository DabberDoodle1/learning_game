#pragma once

#include "word_data.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Game {
public:
    static void setup(unsigned int width, unsigned int height, const char *title);
    static void run();
private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void draw_gui(const WordData* choices[]);

    static GLFWwindow  *window;
    static unsigned int m_width;
    static unsigned int m_height;
};
