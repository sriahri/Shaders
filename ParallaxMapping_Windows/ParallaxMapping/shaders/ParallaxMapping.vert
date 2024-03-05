#version 410 core

/* Uniform variables for Camera */ 
uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

/* Strict Binding for Vertex Attributes */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

/* Parallax Mapping Parameters */
uniform float scale;
uniform float bias;

/* Interpolation Values */
out vec3 interp_VertexPosition;
out vec3 interp_Normal;
out vec2 interp_Texcoord;

/* Parallax Camera Viewing Angle */
out vec3 interp_CameraDirection;

/* Parallax Mapping */
void main(void) {
	interp_VertexPosition = vec3(modelViewMatrix * vec4(position, 1.0f));
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_Texcoord = textureCoordinate.xy;
	
	//----------------------------------------------------------------------------
	// Forming the tbn matrix (tangent space coordinate system)
	//----------------------------------------------------------------------------
	vec3 n = interp_Normal;
	vec3 t = normalize(mat3(normalMatrix) * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	mat3 TBN = mat3(t, b, n);
	
	//----------------------------------------------------------------------------
	// Translate the camera direction into tangent space.
	//----------------------------------------------------------------------------
	interp_CameraDirection = normalize(-interp_VertexPosition);
	interp_CameraDirection *= TBN;
	
    // Perform normal transformation for vertex position
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}