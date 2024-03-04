#version 410 core

/* Uniform variables for Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Light Position passed from C++ */
uniform vec3 lightPosition; 

/* Strict Binding for Cross-hardware Compatibility */ 
layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 normal; 
layout(location = 2) in vec4 tangent; 
layout(location = 3) in vec3 textureCoordinate; 
layout(location = 4) in vec3 color; 
 
/* 
 * Light model information interpolated between each vertex. This information
 * is used to compute the light model within the fragment shader based on the 
 * interpolated vector values.
 */ 
out vec3 interpSurfaceNormal;
out vec3 interpVertexPosition;
out vec3 interpLightPosition;
 
/* Phong Shading */ 
void main(void) { 
	vec4 vPosition = vec4(position, 1.0f); 
	vec4 lPosition = vec4(lightPosition, 1.0f); 
 
	//--------------------------------------------------------------------------
	// Interpolated Light Position and Surface Vectors 
	//--------------------------------------------------------------------------
	interpLightPosition = vec3(modelViewMatrix * lPosition); 
	interpVertexPosition = vec3(modelViewMatrix * vPosition); 
	interpSurfaceNormal = normalize(mat3(normalMatrix) * normal); 
 
	//--------------------------------------------------------------------------
	// Transform the vertex for the fragment shader.
	//--------------------------------------------------------------------------
	gl_Position = projectionMatrix * modelViewMatrix * vPosition; 
}