#pragma once

enum DrawableShapes {
    QUAD = 0
};

class Drawable {
public:
    static void init_VAOs();
    static void delete_VAOs();

    Drawable(DrawableShapes shape): m_shape(shape) {}

    void draw() const;

private:
    DrawableShapes m_shape;
};
