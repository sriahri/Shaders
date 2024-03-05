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

/*
 * Reflections off of a reflective surface are based on the environment around
 * the object and the viewing angle. Therefore the camera position is provided
 * so that the viewing angle can be used to calculate the reflection of the
 * environment around the object.
 */
uniform vec3 cameraPosition;

/*
 * The reflection direction represents the reflection of the environment cube
 * shown on the surface of the object with respect to the current viewing
 * direction. This value is calculated in the vertex shader because the
 * coordinate transformation (based on the the model view matrix) and since the 
 * vertex shader will be executed less frequently than the fragment shader, this
 * provides a more efficient implementation because the interpolation process
 * is more efficient.
 */
out vec3 interp_ReflectDir;

void main(void) {
	//--------------------------------------------------------------------------
	// Determine the reflection direction based on the current camera position
	// and the surface normal. First the vertex position, camera position, and
	// surface normals are transformed into the same coordinate space.
	//--------------------------------------------------------------------------
	vec3 vertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	vec3 cameraPosition = vec3(modelViewMatrix * vec4(cameraPosition, 1.0f));
	vec3 surfaceNormal = normalize(mat3(normalMatrix) * normal);
	
	//--------------------------------------------------------------------------
	// The view direction is based on the vector from the camera position to the
	// current vertex. Since we need to evaluate the reflection across the
	// surface of the object, the reflection direction will be the vector
	// that is interpolated across the surface. Then each interpolated 
	// reflection direction will be used as a 3D texture coordinate to determine
	// which part of the cubemap is reflected by the surface. 
	//--------------------------------------------------------------------------
	vec3 view = normalize(vertexPosition - cameraPosition);
	interp_ReflectDir = reflect(view, surfaceNormal);
	
	//--------------------------------------------------------------------------
	// Transform and project the current vertex to the screen.
	//--------------------------------------------------------------------------
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0f);
}