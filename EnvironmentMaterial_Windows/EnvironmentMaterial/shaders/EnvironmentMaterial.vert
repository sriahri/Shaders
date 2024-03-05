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


/* Uniform variables for the Mesh, Camera, Light */
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

/* 
 * Interpolated light, normal, and fragment position. This is the same as the 
 * light direction specified within the Phong lighting model.
 */
out vec3 interp_Normal;
out vec3 interp_VertexPosition;
out vec3 interp_LightPosition;

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

/* Environment Mapped Material */
void main(void) {
	//--------------------------------------------------------------------------
	// Converting the vector3 position, camera, and light position to vec4 to 
	// match the matrix size for the coordinate transformation with the model 
	// view matrix.
	//--------------------------------------------------------------------------
	vec4 vPosition = vec4(position, 1.0f);
	vec4 lPosition = vec4(lightPosition, 1.0f);
	vec4 cPosition = vec4(cameraPosition, 1.0f);
	
	//--------------------------------------------------------------------------
	// Compared to basic environment mapping, since the environment material
	// is lit ising the Phong lighting model these values must also be provided
	// to the fragment shader.
	//--------------------------------------------------------------------------
	interp_LightPosition = vec3(modelViewMatrix * lPosition);
	interp_VertexPosition = vec3(modelViewMatrix * vPosition);       
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	
	//--------------------------------------------------------------------------
	// The view direction is based on the vector from the camera position to the
	// current vertex. Since we need to evaluate the reflection across the
	// surface of the object, the reflection direction will be the vector
	// that is interpolated across the surface. Then each interpolated 
	// reflection direction will be used as a 3D texture coordinate to determine
	// which part of the cubemap is reflected by the surface. 
	//--------------------------------------------------------------------------
	vec3 cameraPosition = vec3(modelViewMatrix * cPosition);
	vec3 view = normalize(interp_VertexPosition - cameraPosition);
	interp_ReflectDir = reflect(view, interp_Normal);
	
	//--------------------------------------------------------------------------
	// Transform and project the current vertex to the screen.
	//--------------------------------------------------------------------------
	gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}