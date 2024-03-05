#version 410 core

/* Uniform variables for Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Light Position passed from C++ */
uniform vec3 lightPosition;

/* Strict Binding of the Vertex Attributes */ 
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
out vec3 interp_LightPosition;
out vec3 interp_VertexPosition;
out vec3 interp_Normal;
out vec2 interp_Texcoord;

/* TBN Normal Mapping Matrix */
out mat3 TBN;

void main(void) {
	interp_VertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	interp_LightPosition = vec3(modelViewMatrix * vec4(lightPosition, 1.0f));
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
