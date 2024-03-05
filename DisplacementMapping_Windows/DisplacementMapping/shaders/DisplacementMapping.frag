#version 410 core

in vec3 interp_LightPosition;
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

/* Output. Note that gl_FragColor is deprecated in newer GLSL versions. */
out vec4 fragColor;

/* Displacement Mapping */
void main(void) {
	//-------------------------------------------------------------------------- 
	// Wireframe.
	//-------------------------------------------------------------------------- 
	const float scale = 64.0f;
	bvec2 toDiscard = greaterThan(fract(interp_Texcoord * scale), vec2(0.1f, 0.1f));
	if ( all(toDiscard) ) discard;

	//-------------------------------------------------------------------------- 
	// Light and Material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.6f, 0.6f, 0.8f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ka = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 Kd = vec4(0.2f, 0.6f, 0.8f, 1.0f);
	vec4 Ks = vec4(0.6f, 0.7f, 1.0f, 1.0f);
	float shininess = 16.0f;
	
	//-------------------------------------------------------------------------- 
	// Wireframe Color.
	//-------------------------------------------------------------------------- 
	fragColor = Ia * Ka;  
}