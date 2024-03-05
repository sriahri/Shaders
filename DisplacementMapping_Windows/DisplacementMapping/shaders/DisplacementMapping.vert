#version 410 core

/* Uniform variables for Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Light Position passed from C++ */
uniform vec3 lightPosition;

/* Strict Binding for Vertex Attributes */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

/* Displacement Map */
uniform sampler2D heightmapTexture;
uniform float height;

/* Inpterpolated data */
out vec3 interp_LightPosition;
out vec3 interp_VertexPosition;
out vec3 interp_Normal;
out vec2 interp_Texcoord;

/* Displacement Mapping */
void main(void) {
	interp_VertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	interp_LightPosition = vec3(modelViewMatrix * vec4(lightPosition, 1.0f));
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_Texcoord = textureCoordinate.xy;
	
	//---------------------------------------------------------------------------- 
	// Use the height map to displace each vertex according to the direction of
	// its associated normal. The constants are to convert the rgb image into
	// a gray value.
	//---------------------------------------------------------------------------- 
	vec4 heightMap = texture(heightmapTexture, interp_Texcoord);
	float dispValue = 0.3 * heightMap.x + 0.6 * heightMap.y + 0.1 * heightMap.z;
	vec4 dispVector = vec4(normal * dispValue * height, 0.0f) * dispValue;
	vec4 dispPosition = vec4(position, 1.0f) + dispVector;
	
	//-------------------------------------------------------------------------- 
	// Transform the vertex for the fragment shader. 
	//-------------------------------------------------------------------------- 
    gl_Position = projectionMatrix * modelViewMatrix * dispPosition;
}