#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

uniform mat4 uMVP;

out vec3 vColor;

void main() {
    vColor = inColor;
    gl_Position = uMVP * vec4(inPos, 1.0);
}
