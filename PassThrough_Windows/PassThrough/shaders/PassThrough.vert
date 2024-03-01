#version 410 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 textureCoordinate;

void main(void) {
  gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
