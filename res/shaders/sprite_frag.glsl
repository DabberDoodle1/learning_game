#version 460 core

in vec2 texture_pos;

out vec4 frag_color;

uniform sampler2D texture_unit;

void main()
{
    frag_color = texture(texture_unit, texture_pos);
}
