#version 330 core
out vec4 FragColor;
in vec2 tex_coord;

uniform sampler2D color_texture;

void main(){
    FragColor = texture(color_texture, tex_coord);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
