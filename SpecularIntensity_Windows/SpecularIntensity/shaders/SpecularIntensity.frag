#version 410 core

uniform sampler2D specularTexture;

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */
in vec3 interp_LightPosition;
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

/* Output. Note that gl_FragColor is deprecated in newer GLSL versions. */
out vec4 fragColor;

/* Specular Intensity Shading */
void main(void) {
	float intensity = texture(specularTexture, interp_Texcoord).r;
	float ambientIntensityFactor = 0.0005f;
	float diffuseIntensityFactor = 0.07f;
	
	//-------------------------------------------------------------------------- 
	// Light, camera, and reflection direction calculations.
	//-------------------------------------------------------------------------- 
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	//-------------------------------------------------------------------------- 
	// Light and material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//--------------------------------------------------------------------------
	// Red
	//--------------------------------------------------------------------------
	//vec4 Ka = vec4(1.0f, 0.2f, 0.1f, 1.0f);
	//vec4 Kd = vec4(1.0f, 0.2f, 0.1f, 1.0f);
	//vec4 Ks = vec4(1.0f, 0.2f, 0.1f, 1.0f);
	
	//--------------------------------------------------------------------------
	// Blue
	//--------------------------------------------------------------------------
	//vec4 Ka = vec4(0.2f, 0.2f, 1.0f, 1.0f);
	//vec4 Kd = vec4(0.2f, 0.6f, 1.0f, 1.0f);
	//vec4 Ks = vec4(0.1f, 0.2f, 1.0f, 1.0f);
	
	//--------------------------------------------------------------------------
	// Green
	//--------------------------------------------------------------------------
	vec4 Ka = vec4(0.2f, 1.0f, 0.1f, 1.0f);
	vec4 Kd = vec4(0.2f, 1.0f, 0.1f, 1.0f);
	vec4 Ks = vec4(0.2f, 1.0f, 0.1f, 1.0f);
	
	float shininess = 16.0f;
	float flakeShininess = 4.0f;
	
	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);
        
	//-------------------------------------------------------------------------- 
	// Assign the vertex color as the ambient color. 
	//--------------------------------------------------------------------------
	Iambient = Ia * Ka;
	Iambient += vec4(intensity * ambientIntensityFactor);
 
	//-------------------------------------------------------------------------- 
	// Calculate the diffuse component based on the surface normal and the light 
	// direction and add it to the vertex color. 
	//-------------------------------------------------------------------------- 
	float lambertComponent = max(0.0f, dot(interp_Normal, l)); 
	Idiffuse = (Id * Kd) * lambertComponent;
	Idiffuse += vec4(intensity * diffuseIntensityFactor);
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. The reflection vector is scaled by the provided
	// intensity value read from the specular map.
	//-------------------------------------------------------------------------
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess);
	Ispecular += pow(max(dot(intensity * r, c), 0.0f), flakeShininess);
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex. 
	//--------------------------------------------------------------------------
	fragColor = Iambient + Idiffuse + Ispecular;  
}