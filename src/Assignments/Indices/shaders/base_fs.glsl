#version 410

in vec3 vColor;

layout(location=0) out vec4 vFragColor;

void main() {
    
    vFragColor = vec4(vColor,1);
}
