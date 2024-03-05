#version 410 core

/* 
 * Fog structure Info
 * 
 * The min distance represents where the distance at which the fog starts in
 * front of the camera and the max distance represents the distance from the
 * camera at which objects will be completely covered in fog (not visible).
 */
struct FogInfo {
	float maxDist;
	float minDist;
	vec3 color;
};

uniform FogInfo fog;

/* 
 * Light model information interpolated between each vertex. This information is 
 * used to compute the light model within the fragment shader based on the 
 * interpolated vector values. 
 */
in vec3 interp_Normal;
in vec3 interp_VertexPosition;
in vec3 interp_LightPosition;

/* Final fragment color */
out vec4 fragColor;

/* Fog Shading */
void main(void) {
	//-------------------------------------------------------------------------- 
	// Define the fog gradient and the depth of the current fragment. Based on 
	// this distance, the value of the fog component will vary between 0 and 1.
	// If the component is 0 then the fog will not be visible; otherwise if
	// the component is 1 then the fog will completely fill the fragment making
	// the object not visible.
	//-------------------------------------------------------------------------- 
	float dist = abs(interp_VertexPosition.z);
	float fogFactor = (fog.maxDist - dist) / (fog.maxDist - fog.minDist);
	fogFactor = clamp(fogFactor, 0.0f, 1.0f);
	
	//-------------------------------------------------------------------------- 
	// Light, camera, and reflection direction calculations.
	//-------------------------------------------------------------------------- 
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	//-------------------------------------------------------------------------- 
	// Light Properties
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//-------------------------------------------------------------------------- 
	// Material Properties
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
	float lambertComponent = max(0.0f, dot(interp_Normal, l)); 
	Idiffuse = (Id * Kd) * lambertComponent; 
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. 
	//------------------------------------------------------------------------- 
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess); 
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex and add the
	// fog factor component. This is based on the value of the mix ratio defined
	// by the fogFactor value (between 0 and 1).
	//--------------------------------------------------------------------------
	vec4 adsComponent = Iambient + Idiffuse + Ispecular;
	fragColor = vec4(mix(fog.color, vec3(adsComponent), fogFactor), 1.0f);
}