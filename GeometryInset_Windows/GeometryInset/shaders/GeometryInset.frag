#version 410 core

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */
in vec3 interp_Normal;
in vec3 interp_VertexPosition;
in vec3 interp_LightPosition;

out vec4 fragColor;

/* Geometry Inset Shader (Phong Shading) */
void main(void) {
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	//-------------------------------------------------------------------------- 
	// Light and material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	vec4 Ka = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	float shininess = 16.0f;

	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);

	//-------------------------------------------------------------------------- 
	// Phong lighting.
	//--------------------------------------------------------------------------
	Iambient = Ia * Ka;
	float lambertComponent = max(0.0f, dot(interp_Normal, l)); 
	Idiffuse = (Id * Kd) * lambertComponent; 
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess); 
	
	fragColor = Iambient + Idiffuse + Ispecular;  
}