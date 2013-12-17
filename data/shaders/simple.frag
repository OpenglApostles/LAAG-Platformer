#version 330 core

uniform sampler2D text;
in vec2 UV;

void main(){
    vec4 mat_dif = texture2D( text, UV ).rgba;
    gl_FragColor.rgba = vec4(mat_dif)+vec4(1,1,1,.1);
}
