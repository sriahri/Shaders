#version 410 core

/* Textures */
uniform sampler2D diffuseTexture;
uniform sampler2D heightmapTexture;

/* Parallax Mapping Parameters */
uniform float scale;
uniform float bias;

/* Interpolation Values */
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

/* Parallax Camera Viewing Angle */
in vec3 interp_CameraDirection;

/* Output. Note that gl_FragColor is deprecated in newer GLSL versions. */
out vec4 fragColor;

/* Parallax Mapping */
void main(void) {
	//----------------------------------------------------------------------------
	// Extract the surface height from the height texture (only the red channel).
	//----------------------------------------------------------------------------
	float height = texture(heightmapTexture, interp_Texcoord).r;
	float v = height * scale - bias;
	
	//----------------------------------------------------------------------------
	// Based on the eye position, translate the height into the texture offset.
	//----------------------------------------------------------------------------
	vec3 eye = normalize(interp_CameraDirection);
	vec2 offsetCoords = interp_Texcoord - (eye.xy * v);
	
	//----------------------------------------------------------------------------
	// Use the offset coordinates to access the diffuse texture.
	//----------------------------------------------------------------------------
	vec4 offsetDiffuseTexture = texture2D(diffuseTexture, offsetCoords);
	
	fragColor = offsetDiffuseTexture; 
}