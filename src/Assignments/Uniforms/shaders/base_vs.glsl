#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 a_vertex_color;


out vec3 vColor;


void main() {
    gl_Position = a_vertex_position;
    vColor = a_vertex_color;

}

