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

/* Uniform variables for the Mesh, Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;
uniform vec3 cameraPosition;

/* Reflection and refraction directions to be interpolated across the surface. */
out vec3 interp_ReflectDir;
out vec3 interp_RefractDir;

/* Refraction Shader */
void main(void) {
	vec4 vPosition = vec4(position, 1.0f);

	vec3 vertexPosition = vec3(modelViewMatrix * vPosition);       
	vec3 surfaceNormal = normalize(mat3(normalMatrix) * normal);
	vec3 cameraPosition = vec3(modelViewMatrix * vec4(cameraPosition, 1.0f));
	
	//-------------------------------------------------------------------------- 
	// The reflection and refraction directions are view dependent.
	//-------------------------------------------------------------------------- 
	vec3 view = normalize(cameraPosition - vertexPosition);
	
	//-------------------------------------------------------------------------- 
	// Calculate the reflection and refraction directions to be usedas
	// cubemap texture coordinates. The constant 'eta' values is based on the 
	// index of refraction for a particular material (these values can be 
	// looked up using any physics textbook or on the internet).
	//-------------------------------------------------------------------------- 
	interp_ReflectDir = reflect(-view, surfaceNormal);
	float eta = 1.0 / 1.5f;
	interp_RefractDir = refract(-view, surfaceNormal, eta);
	
	//--------------------------------------------------------------------------
	// Transform and project the current vertex to the screen.
	//--------------------------------------------------------------------------
	gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}