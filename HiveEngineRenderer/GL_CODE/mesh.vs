#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;

out vec4 f_color;

uniform mat4 view;
uniform mat4 transform;

void main(){
	f_color = aCol;
    gl_Position = view * vec4(aPos, 1.0);
}