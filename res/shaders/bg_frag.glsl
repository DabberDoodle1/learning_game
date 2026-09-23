#version 450 core

out vec4 frag_color;

// uniform float pattern_size;
const float ps = 1280.0f / 32.0f;

void square_pattern();

void main()
{
    square_pattern();
}

void square_pattern()
{
    bool is_dark = (mod(gl_FragCoord.x, 2 * ps) > ps) ^^ (mod(gl_FragCoord.y, 2 * ps) > ps);

    if (is_dark) {
        frag_color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    } else {
        frag_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    }
}
