#version 450 core

out vec4 frag_color;

void main()
{
    bool is_dark = (mod(gl_FragCoord.x, 160.0f) > 80.0f) ^^ (mod(gl_FragCoord.y, 160.0f) > 80.0f);

    if (is_dark) {
        frag_color = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    } else {
        frag_color = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    }
}
