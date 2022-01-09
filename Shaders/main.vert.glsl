#version 450 core

const vec2 vertices[] = {
    vec2(-2.0, -2.0),
    vec2(-1.0,  4.0),
    vec2( 4.0, -1.0)
};
layout(location=0)uniform float akd;
out vec2 FragCord;

void main() {
    gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
    FragCord = vec2(gl_Position.x*akd, gl_Position.y);
}