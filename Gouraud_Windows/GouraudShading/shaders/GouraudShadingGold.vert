#version 410 core

/* Uniform variables for Camera and Light Direction */ 
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

/* Strict Binding for Cross-hardware Compatability */
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

/* 
 * Vector4 that contains the (R,G,B,A) components of the calculated light color 
 * for this vertex. This is the color value that will be interpolated across the 
 * faces adjacent to this vertex. 
 */
out vec4 interpShadedColor;

/* Gouraud Shading */
void main() {
	vec4 vPosition = vec4(position, 1.0f);
	vec4 lPosition = vec4(lightPosition, 1.0f);
	
	//-------------------------------------------------------------------------- 
	// Translate the vertex, light, and normal into eye coordinates.
	//-------------------------------------------------------------------------- 
	vec3 eyeVertexPosition = vec3(modelViewMatrix * vPosition);
	vec3 eyeLightPosition = vec3(modelViewMatrix * lPosition);
	vec3 eyeNormal = normalize(mat3(normalMatrix) * normal);
	
	//-------------------------------------------------------------------------- 
	// Light, camera, and reflection direction calculations.
	//-------------------------------------------------------------------------- 
	vec3 l = normalize(eyeLightPosition - eyeVertexPosition);
	vec3 c = normalize(-eyeVertexPosition);
	vec3 r = normalize(-reflect(l, eyeNormal));
	
	//-------------------------------------------------------------------------- 
	// Light and material properties. 
	//-------------------------------------------------------------------------- 
	vec4 Ia = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 Id = vec4(1.0f, 1.0f, 0.9f, 1.0f);
	vec4 Is = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	
	vec4 Ka = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 Kd = vec4(1.0f, 0.7f, 0.0f, 1.0f);
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
	float lambertComponent = max(0.0f, dot(eyeNormal, l)); 
	Idiffuse = (Id * Kd) * lambertComponent; 
	
	//-------------------------------------------------------------------------- 
	// Calculate the specular component based on the camera position and 
	// reflection direction. 
	//------------------------------------------------------------------------- 
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess); 
	
	//-------------------------------------------------------------------------- 
	// Calculate the final ADS light value for this vertex. 
	//--------------------------------------------------------------------------
	interpShadedColor = Iambient + Idiffuse + Ispecular;
	
	//-------------------------------------------------------------------------- 
	// Transform the vertex for the fragment shader. 
	//-------------------------------------------------------------------------- 
    gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}
