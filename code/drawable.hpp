#pragma once

enum DrawableShapes {
    QUAD = 0
};

struct Drawable {
    static void init_VAOs();
    static void delete_VAOs();

    void draw(DrawableShapes shape) const;
};
