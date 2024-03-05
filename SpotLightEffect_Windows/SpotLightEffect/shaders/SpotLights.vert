#version 410 core

const int LIGHT_COUNT = 4;

/* Strict Binding for Cross-hardware Compatability */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

/* Uniform variables for Camera and Light Direction */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

struct SpotLight {
	vec3 position;
	vec3 target;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float exponent;
	float cutoff;
};

uniform SpotLight lights[LIGHT_COUNT];

out vec3 interp_VertexPosition;
out vec3 interp_Normal;
out vec2 interp_Texcoord;

/* TBN Matrix for transforming light direction to tangent space. */
out mat3 TBN;

void main(void) {
    interp_VertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_Texcoord = textureCoordinate.xy;
	
	// TBN Matrix Formulation
	vec3 n = interp_Normal;
	vec3 t = normalize(mat3(normalMatrix) * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	TBN = transpose(mat3(t, b, n));

	// Transform the vertex for the fragment shader. 
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0f);
}