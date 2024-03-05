#version 410 core

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

struct SpotLight {
	vec3 position;
	vec3 target;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float exponent;
	float cutoff;
};

uniform SpotLight light;

in mat3 TBN;
in vec3 interp_VertexPosition;
in vec3 interp_Normal;
in vec2 interp_Texcoord;

out vec4 fragColor;

vec4 ComputeSpotlight(vec4 diffuse_sample, vec4 normal_sample, vec4 specular_sample) {
	vec4 Ia = vec4(light.ambient, 1.0f); 
	vec4 Id = vec4(light.diffuse, 1.0f); 
	vec4 Is = vec4(light.specular, 1.0f);
	vec4 Ka = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ks = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	float shininess = 4.0f;
	float exposure = 1.0f;
	
	vec3 light_position = vec3(modelViewMatrix * vec4(light.position, 1.0f));
	vec3 light_target = vec3(modelViewMatrix * vec4(light.target, 1.0f));
    vec3 direction = normalize(light_target - light_position);
	
	vec3 l = normalize(light_position - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	float angle = acos(dot(-l, direction));
    float cutoff = radians(clamp(light.cutoff, 0.0, 90.0));
	
	// Retrieve and decode (denormalize) the normal map normal (n) and then
	// compute the light direction in tangent space for the ADS model.
	vec3 pixel_normal = normalize(normal_sample.xyz * 2.0 - 1.0);
	vec3 pixel_light = TBN * l;
	
	vec4 Iambient, Idiffuse, Ispecular;
	Iambient = Ia * Ka;
	
	if( angle < cutoff ) {
        float spotFactor = pow(dot(-l, direction), light.exponent);
		float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
		Idiffuse = spotFactor * (Id*Kd) * diffuse_sample  * lambertComponent;
		Ispecular = spotFactor * (Is*Ks) * specular_sample * pow(max(dot(r, c), 0.0), shininess);
		return spotFactor * (Iambient + Idiffuse + Ispecular);
		
    } else {
		float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
        return Iambient * diffuse_sample * exposure * lambertComponent;
    }
}

void main(void) {
	vec4 diffuse_sample = texture(diffuseTexture, interp_Texcoord);
	vec4 normal_sample = texture(normalTexture, interp_Texcoord);
	vec4 specular_sample = texture(specularTexture, interp_Texcoord);
	
	fragColor = ComputeSpotlight(diffuse_sample, normal_sample, specular_sample);
}
