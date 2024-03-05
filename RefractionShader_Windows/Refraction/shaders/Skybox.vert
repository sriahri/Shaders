#version 410 core

/* 
 * Geometric data from the Mesh. These components are only used within the
 * vertex shader to define the geometry and perform coordinate transformations.
 */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;
uniform vec3 cameraPosition;

out vec3 reflectDir;

void main(void) {
	vec4 vPosition = vec4(position, 1.0f);

	vec3 vertexPosition = vec3(modelViewMatrix * vPosition);       
	vec3 wCameraPosition = vec3(modelViewMatrix * vec4(cameraPosition, 1.0f));
	vec3 view = normalize(vertexPosition - wCameraPosition);
	reflectDir = vertexPosition;
	
	gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}