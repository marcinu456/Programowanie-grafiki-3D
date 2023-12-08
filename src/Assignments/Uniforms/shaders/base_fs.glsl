#version 420
in vec3 vColor;

layout(location=0) out vec4 vFragColor;
layout(binding = 0, std140) uniform Modifier {
    float strength;
    vec3  color; 
};

layout(std140, binding=1) uniform Transformations {
 vec2 scale;
 vec2 translation;
 mat2 rotation;
 };


void main() {
    
    vFragColor = vec4(vColor*color,1);
    vFragColor[1] = vFragColor[1]*strength;
    vFragColor[2] = vFragColor[2]*strength;
    vFragColor[3] = vFragColor[3]*strength;
}
