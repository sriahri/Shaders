#version 410 core

const int LIGHT_COUNT = 3;

/* Uniform variables for Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

// TODO: Define light structure

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// TODO: Define light uniform array (ex. lights[LIGHT_COUNT])
uniform Light lights[LIGHT_COUNT];




/* Strict Binding for Vertex Attributes */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

/* 
 * Light model information interpolated between each vertex. This information
 * is used to compute the light model within the fragment shader based on the 
 * interpolated vector values. The texture coordinate must be provided to map
 * texture coordinates to fragments.
 */ 
out vec3 interp_LightPositions[LIGHT_COUNT];
out vec3 interp_VertexPosition;
out vec3 interp_Normal;
out vec2 interp_Texcoord;

/* TBN Matrix for transforming light direction to tangent space. */
out mat3 TBN;

/* Specular Mapping */
void main(void) {
	interp_VertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	
	// TODO: For each light compute interp_LightPosition
	
	for (int i = 0; i < LIGHT_COUNT; ++i) {
        interp_LightPositions[i] = vec3(modelViewMatrix * vec4(lights[i].position, 1.0f));
    }
	
	
	
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_Texcoord = textureCoordinate.xy;
	
	// TBN Matrix Formulation
	vec3 n = interp_Normal;
	vec3 t = normalize(mat3(normalMatrix) * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	TBN = transpose(mat3(t, b, n));
	
	// Perform normal transformation for vertex position
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}

