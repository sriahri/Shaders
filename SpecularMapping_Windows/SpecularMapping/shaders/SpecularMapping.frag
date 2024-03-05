#version 410 core

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
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

/* TBN Matrix for transforming light direction to tangent space. */
in mat3 TBN;

/* Output. Note that gl_FragColor is deprecated in newer GLSL versions. */
out vec4 fragColor;

/* Specular Mapping */
void main(void) {
	vec4 diffuse_sample = texture(diffuseTexture, interp_Texcoord);
	vec4 normal_sample = texture(normalTexture, interp_Texcoord);
	vec4 specular_sample = texture(specularTexture, interp_Texcoord);
	
	// View and reflection normals for ADS illumination model.
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	// Retrieve and decode (denormalize) the normal map normal (n)
	vec3 pixel_normal = normalize(normal_sample.xyz * 2.0 - 1.0);
	
	// Compute the light direction in tangent space for the ADS model.
	vec3 pixel_light = TBN * l;
	
	//-------------------------------------------------------------------------- 
	// Light and Material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ka = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 Kd = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 16.0f;
	
	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);

	Iambient = Ia * Ka;
 
	//-------------------------------------------------------------------------- 
	// Compute diffuse component with texture sampling.
	//-------------------------------------------------------------------------- 
	float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
	Idiffuse = diffuse_sample * (Id * Kd) * lambertComponent;
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. The reflection vector is scaled by the provided
	// intensity value read from the specular map.
	//-------------------------------------------------------------------------
	Ispecular = specular_sample * (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess);
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex. 
	//--------------------------------------------------------------------------
	fragColor = Iambient + Idiffuse + Ispecular;  
}