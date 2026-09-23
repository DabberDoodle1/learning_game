#version 450 core

layout (location = 0) in vec2 vertex_pos;

uniform mat4 model;
uniform mat4 projection;

out vec2 texture_pos;

void main()
{
    gl_Position = projection * model * vec4(vertex_pos, 1.0f, 1.0f);

    // Normalize vertex pos to get texture coords
    texture_pos = vertex_pos + 0.5f;
}
