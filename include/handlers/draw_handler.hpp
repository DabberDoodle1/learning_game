#pragma once

#include <glm/ext/matrix_float4x4.hpp>

enum DrawableShape {
    QUAD = 0
};

namespace DrawHandler {
    void init_VAOs();
    void delete_VAOs();
    void draw(DrawableShape shape);
};

struct Drawable {
    Drawable(float _x, float _y, float _w, float _h): x(_x), y(_y), w(_w), h(_h) {}

    glm::mat4 get_model();

    float x;
    float y;
    float w;
    float h;
};
