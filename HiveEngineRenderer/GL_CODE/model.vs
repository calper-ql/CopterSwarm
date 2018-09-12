#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex_coord_in;
layout (location = 2) in mat4 transform;

out vec2 tex_coord;

uniform mat4 view;

void main(){
	tex_coord = tex_coord_in;
    gl_Position = view * transform * vec4(aPos, 1.0);
}