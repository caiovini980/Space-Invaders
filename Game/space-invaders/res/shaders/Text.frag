#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_UV;

uniform sampler2D u_Text;
uniform vec3 u_TextColor;

void main()
{
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(u_Text, v_UV).r);
    o_Color = vec4(u_TextColor, 1.f) * sampled;
}