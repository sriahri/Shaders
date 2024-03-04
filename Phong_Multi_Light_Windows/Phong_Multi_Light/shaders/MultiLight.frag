#version 410 core

const int LIGHT_COUNT = 4;

// (1) Define the light structure. This should match the definition
// in the vertex shader.
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// (2) TODO: Create an array of Light structure for 4 lights. It should be a uniform.
uniform Light lights[LIGHT_COUNT];
/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */ 
in vec3 interpSurfaceNormal;
in vec3 interpVertexPosition;

// (3) TODO: Create an input array for interpolated light positions from the vertex shader.
in vec3 interpLightPositions[LIGHT_COUNT];
/* Output frag color. */
out vec4 fragColor;

// (4) TODO: Create a function to compute the Phong fragColor based on
// as single light. The return type should be vec4.
 vec4 CalculatePhongLight(Light light, vec3 fragPos, vec3 normal) {
    vec3 lightDir = normalize(fragPos - interpVertexPosition);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = normalize(-reflect(lightDir, normal));
    vec3 viewDir = normalize(-interpVertexPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);

    vec3 ambient = light.ambient * 0.3;
    vec3 diffuse = light.diffuse * diff * 0.8;
    vec3 specular = light.specular * spec * 0.7;

    //-------------------------------------------------------------------------- 
	// Light properties (hard-coded).
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(ambient, 1.0f);
	vec4 Id = vec4(diffuse, 1.0f); 
	vec4 Is = vec4(specular, 1.0f); 

	//-------------------------------------------------------------------------- 
	// Material properties (hard-coded).
	//-------------------------------------------------------------------------- 
	vec4 Ka = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	float shininess = 16.0f; 
 
	vec4 Iambient = vec4(0.0f); 
	vec4 Idiffuse = vec4(0.0f); 
	vec4 Ispecular = vec4(0.0f); 
 
	//-------------------------------------------------------------------------- 
	// Assign the vertex color as the ambient color. 
	//-------------------------------------------------------------------------- 
	Iambient = Ia * Ka; 
 
	//-------------------------------------------------------------------------- 
	// Calculate the diffuse component based on the surface normal and the light 
	// direction and add it to the vertex color. 
	//-------------------------------------------------------------------------- 
	float lambertComponent = max(0.0f, dot(normal, lightDir));
	Idiffuse = (Id * Kd) * lambertComponent;

	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and
	// reflection direction.
	//------------------------------------------------------------------------- 
	Ispecular = (Is * Ks) * pow(max(dot(reflectDir, viewDir), 0.0f), shininess);

    vec4 result = Iambient + Idiffuse + Ispecular;
    return result;
}

vec4 CalculateTotalPhongLight() {
    vec3 totalLight = vec3(0.0);

    for (int i = 0; i < LIGHT_COUNT; ++i) {
        totalLight += CalculatePhongLight(lights[i], interpLightPositions[i], interpSurfaceNormal).xyz;
    }

    return vec4(totalLight, 1.0);
}

void main(void) {
	// (5) TODO: Compute the Phong component from each light. It may be 
	// useful to define a new function that computes the contribution
	// from each light and then adds them to the result. 
	//
	// For the Phong model, the resulting fragColor will be the linear
	// combination of each light. For example, the resulting fragColor
	// intensity for 2 lights would be computed as follows:
	//
	// fragColor = (I_ambient0 + I_diffuse0 + I_specular0) + (I_ambient1 + I_diffuse1 + I_specular1)
	// 
	// The light equtaions are the same for each light, so the basic Phong
	// model can be used on each light separately. This equation can be extended
	// to all 4 lights.

	// fragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    fragColor = CalculateTotalPhongLight();
}
