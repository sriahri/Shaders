#version 410 core

const int LIGHT_COUNT = 4;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

uniform samplerCube cubeMap;
struct SpotLight {
	vec3 position;
	vec3 target;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float exponent;
	float cutoff;
};

uniform SpotLight lights[LIGHT_COUNT];
in vec3 interp_Normal;
in vec3 interp_VertexPosition;
in vec3 interp_LightPosition[LIGHT_COUNT];

in vec2 interp_Texcoord;

in mat3 TBN;
in vec3 interp_ReflectDir;

out vec4 fragColor;

// TODO: Setup environment mapping and add the lighting 
// contribution from each spotlight. The best way to approach this
// is to create a function that returns a vec4 and takes an index
// of the light to compute:
// vec4 ComputeSpotlight(int light_index, ..) {
//		...
//		return spotlight_contribution;
// }

vec4 ComputeSpotlight(int light_index, vec4 diffuse_sample, vec4 normal_sample, vec4 specular_sample) {
    //--------------------------------------------------------------------------
	// Determine the environment map fragment color based on the reflection
	// direction.
	//--------------------------------------------------------------------------
	vec4 cubeMapColor = texture(cubeMap, interp_ReflectDir);
	vec3 light_position = vec3(modelViewMatrix * vec4(lights[light_index].position, 1.0f));
	vec3 light_target = vec3(modelViewMatrix * vec4(lights[light_index].target, 1.0f));
    vec3 direction = normalize(light_target - light_position);
	//--------------------------------------------------------------------------
	// Light vector calculations from Phong shading. These are used to apply
	// the same ADS light model to the environment mapped object.
	//--------------------------------------------------------------------------
	vec3 l = normalize(interp_LightPosition[light_index] - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	//--------------------------------------------------------------------------
	// The color of the light is assumed to be white.
	//--------------------------------------------------------------------------
	vec4 Ia = vec4(lights[light_index].ambient, 1.0f); 
	vec4 Id = vec4(lights[light_index].diffuse, 1.0f); 
	vec4 Is = vec4(lights[light_index].specular, 1.0f);
	vec4 Ka = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Ks = vec4(0.3f, 0.3f, 0.3f, 1.0f);
	float shininess = 4.0f;
	float exposure = 1.0f;

	float angle = acos(dot(-l, direction));
    float cutoff = radians(clamp(lights[light_index].cutoff, 0.0, 90.0));
	
	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);

	//--------------------------------------------------------------------------
	// ADS Lighting Model applied to the environment map base texture. This
	// implementation is based on the Phong lighting model. The only difference
	// between using a normal diffuse texture and the environment map is that
	// the diffuse color is set to the value provided by the cubemap rather
	// than the diffuse texture.
	//--------------------------------------------------------------------------
	vec3 pixel_normal = normalize(normal_sample.xyz * 2.0 - 1.0);
	vec3 pixel_light = TBN * l;
	Iambient = cubeMapColor * Ia * Ka;
	if( angle < cutoff ) {
        float spotFactor = pow(dot(-l, direction), lights[light_index].exponent);
		float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
		Idiffuse = cubeMapColor * spotFactor * (Id*Kd) * diffuse_sample  * lambertComponent;
		Ispecular = spotFactor * (Is*Ks) * specular_sample * pow(max(dot(r, c), 0.0), shininess);
		return spotFactor * (Iambient + Idiffuse + Ispecular);
		
    } else {
		float lambertComponent = clamp(dot(pixel_normal, pixel_light), 0.0f, 1.0f);
        return Iambient * diffuse_sample * exposure * lambertComponent;
    }
}


void main(void) {
	// TODO: See Environment Mapping
    vec4 diffuse_sample = texture(diffuseTexture, interp_Texcoord);
	vec4 normal_sample = texture(normalTexture, interp_Texcoord);
	vec4 specular_sample = texture(specularTexture, interp_Texcoord);

	vec4 total_light = vec4(0.0);
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		total_light += ComputeSpotlight(i, diffuse_sample, normal_sample, specular_sample);
	}
    fragColor = total_light;
}
