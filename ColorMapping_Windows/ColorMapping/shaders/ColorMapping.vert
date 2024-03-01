#version 410 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 textureCoordinate;

out vec3 normalColor;

void main(void) {
	vec4 vertexPos = vec4(position, 1.0);
	normalColor = normalize(normal);
	gl_Position = projectionMatrix * modelViewMatrix * vertexPos;
}
