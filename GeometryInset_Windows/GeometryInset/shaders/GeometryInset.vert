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

/* View Matrices */
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Output vertex (sent to the Geometry Shader) */
out Vertex {
	vec3 position;
	vec3 normal;
	vec4 tangent;
	vec3 textureCoord;
	vec3 color;
} vertex;

/* Geometry Inset Shader */
void main(void) {
	vertex.position = position;
	vertex.normal = normal;
	vertex.tangent = tangent;
	vertex.textureCoord = textureCoordinate;
	vertex.color = color;

    gl_Position = vec4(position, 1.0f);
}
