#version 410 core

uniform samplerCube cubeMap;

in vec3 reflectDir;

out vec4 fragColor;

void main(void) {
	vec4 cubeMapColor = texture(cubeMap, reflectDir);
	fragColor = cubeMapColor;
}
