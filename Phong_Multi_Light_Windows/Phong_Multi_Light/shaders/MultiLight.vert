#version 410 core

const int LIGHT_COUNT = 4;

/* Uniform variables for Camera. */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Strict Binding for Cross-hardware Compatibility */ 
layout(location = 0) in vec3 position; 
layout(location = 1) in vec3 normal; 
layout(location = 2) in vec4 tangent; 
layout(location = 3) in vec3 textureCoordinate; 
layout(location = 4) in vec3 color; 

// (1) TODO: Create a new light structure to organize data. Should closely match C++ struct.

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



// (2) TODO: Create an array of Light structure for 4 lights. It should be a uniform.
uniform Light lights[LIGHT_COUNT];

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */ 
out vec3 interpSurfaceNormal;
out vec3 interpVertexPosition;

// (3) TODO: interpolation vec4 for each light (see Phong example)
out vec3 interpLightPositions[LIGHT_COUNT];
 
void main(void) { 
	// (4) Need to compute interpolated light position for all lights.
	// For light 0 this may look like:
	// vec4 lpositions[LIGHT_COUNT];
	// lpositions[0] = vec4(lights[0].position, 1.0f);
	// interp_light_positions[0] = vec3(modelViewMatrix * lpositions[0]);
	// ...

	//---------------------------------------------------------------------------- 
	// Vertex position and interpolated surface vector.
	//----------------------------------------------------------------------------
	vec4 vPosition = vec4(position, 1.0f);
	interpVertexPosition = vec3(modelViewMatrix * vPosition); 
	interpSurfaceNormal = normalize(mat3(normalMatrix) * normal); 

	for (int i = 0; i < LIGHT_COUNT; ++i) {
        vec4 lightPosition = vec4(lights[i].position, 1.0f);
        interpLightPositions[i] = vec3(modelViewMatrix * lightPosition);
    }
	gl_Position = projectionMatrix * modelViewMatrix * vPosition; 
}