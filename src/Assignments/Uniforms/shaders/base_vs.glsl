#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 a_vertex_color;
layout(location=2) in  mat2 rotation;
layout(location=3) in  vec2 translation;
layout(location=4) in  float scale;

out vec3 vColor;


void main() {
    gl_Position = a_vertex_position;
    vColor = a_vertex_color;
    gl_Position.xy = rotation*(scale*a_vertex_position.xy)+translation;
    gl_Position.zw = a_vertex_position.zw;  
}

