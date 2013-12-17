#version 330 core

layout(location = 0) in vec2 vert;
layout(location = 1) in vec2 uv;

uniform vec2 pos;
uniform float inv_size, font_size;
uniform mat4 m, v, p, mvp;

out vec2 UV;

void main(){
    gl_Position = mvp * vec4(vert,1,1);
    //gl_Position = vec4(vert,1,1);
    //gl_Position.xy = vert+pos;
    UV = vec2(uv.x,uv.y);
}