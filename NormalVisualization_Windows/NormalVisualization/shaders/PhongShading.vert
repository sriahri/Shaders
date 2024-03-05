#version 410 core

/* Strict Binding for Cross-hardware Compatability */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */ 
out vec3 interp_Normal;
out vec3 interp_VertexPosition;
out vec3 interp_LightPosition;

/* Phong Shading */
void main(void) {
	vec4 vPosition = vec4(position, 1.0f);
	vec4 lPosition = vec4(lightPosition, 1.0f);
	
	interp_LightPosition = vec3(modelViewMatrix * lPosition);
	interp_VertexPosition = vec3(modelViewMatrix * vPosition);       
	interp_Normal = normalize(mat3(normalMatrix) * normal);

	gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}