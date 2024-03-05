#version 410 core

/* Loaded Normal Map. Must be bound, active OpenGL Texture. */
uniform sampler2D normalTexture;

in vec3 interp_LightPosition;
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

/* TBN Matrix for transforming light direction to tangent space. */
in mat3 TBN;

/* Output. Note that gl_FragColor is deprecated in newer GLSL versions. */
out vec4 fragColor;
 
void main() {
	vec4 normal_sample = texture(normalTexture, interp_Texcoord);
	
	// View and reflection normals for ADS illumination model.
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	// Retrieve and decode (denormalize) the normal map normal (n)
	vec3 pixel_normal = normalize(normal_sample.xyz * 2.0 - 1.0);
	
	// Compute the light direction in tangent space for the ADS model.
	vec3 pixel_light = TBN * l;
	
	// Basic material properties.
	vec4 Ia = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	vec4 Id = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ka = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 4.0f;

	// Basic definition for the ambient component.
	vec4 Iambient = Ia * Ka;
	
	// Compute the diffuse component using the tangent space normal from the normal map.
	float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
	vec4 Idiffuse = (Id * Kd) * lambertComponent;

	fragColor = Iambient + Idiffuse;
}
