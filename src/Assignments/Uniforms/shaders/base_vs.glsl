#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in  vec3 a_vertex_color;

out vec3 vColor;

layout(std140, binding = 1) uniform Transformations {
    mat2 rotation;
    vec2 translation;
    vec2 scale;
};

void main() {
    gl_Position.xy = rotation * ( scale * a_vertex_position.xy) + translation;
    gl_Position.zw = a_vertex_position.zw;
    vColor = a_vertex_color;
}
