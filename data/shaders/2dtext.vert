#version 330 core

layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 uv;

uniform mat4 mvp;

out vec2 UV;

void main(){
    gl_Position = mvp * vec4(vert,1,1);
    UV = vec2(uv.x,uv.y);
}