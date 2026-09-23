#version 460 core

in vec2 texture_pos;

out vec4 frag_color;

uniform sampler2D texture_unit;

void main()
{
    frag_color = texture(texture_unit, texture_pos);

    if (frag_color.w == 0.0f) {
        frag_color = vec4(0.686f, 0.51f, 0.392f, 1.0f);
    }
}
