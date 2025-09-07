#version 330 core

layout (location = 0) in vec3 inPos;    // 원점 주변의 축 기하 (-L ~ +L)
layout (location = 1) in vec3 inColor;  // 각 축 색상

uniform mat4 uView, uProj, uModel;      // uModel = translate(pos) * scale(L)

out vec3 vColor;

void main() {
    vColor = inColor;
    gl_Position = uProj * uView * uModel * vec4(inPos, 1.0);
}
