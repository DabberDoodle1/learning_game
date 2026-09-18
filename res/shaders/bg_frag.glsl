#version 450 core

out vec4 frag_color;

uniform float pattern_size;

void square_pattern();

void main()
{
    square_pattern();
}

void square_pattern()
{
    bool is_dark = (mod(gl_FragCoord.x, 2 * pattern_size) > pattern_size) ^^ (mod(gl_FragCoord.y, 2 * pattern_size) > pattern_size);

    if (is_dark) {
        frag_color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    } else {
        frag_color = vec4(0.27f, 0.27f, 0.27f, 1.0f);
    }
}
