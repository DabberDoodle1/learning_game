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
