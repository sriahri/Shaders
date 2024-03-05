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

/* Input light position */
uniform vec3 lightPosition;

/* Interpolated data for Phong lighting */
out vec3 interp_Normal;
out vec3 interp_VertexPosition;
out vec3 interp_LightPosition;
out vec2 interp_TextureCoord;

/* TBN Normal Mapping Matrix */
out mat3 TBN;

/* Texture Blending */
void main(void) {
	vec4 vPosition = vec4(position, 1.0f);
	vec4 lPosition = vec4(lightPosition, 1.0f);
	
	interp_LightPosition = vec3(modelViewMatrix * lPosition);
    interp_VertexPosition = vec3(modelViewMatrix * vPosition);       
    interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_TextureCoord = vec2(textureCoordinate);
	
	// TBN Matrix Formulation
	vec3 n = interp_Normal;
	vec3 t = normalize(mat3(normalMatrix) * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	TBN = transpose(mat3(t, b, n));
	
	//--------------------------------------------------------------------------
	// Transform and project the current vertex to the screen.
	//--------------------------------------------------------------------------
    gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}
