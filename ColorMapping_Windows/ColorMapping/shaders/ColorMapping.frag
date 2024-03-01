#version 410 core

in vec3 normalColor;
out vec4 color;
 
void main() {
	color = vec4(normalColor, 1.0f);
}
