#version 410 core

const int LIGHT_COUNT = 4;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec3 textureCoordinate;
layout (location = 4) in vec3 color;

uniform mat4 modelViewMatrix; 
uniform mat4 projectionMatrix; 
uniform mat4 normalMatrix;

uniform vec3 cameraPosition;

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

out vec3 interp_Normal;
out vec3 interp_VertexPosition;
out vec3 interp_LightPosition[LIGHT_COUNT];

out vec3 interp_ReflectDir;

out vec2 interp_Texcoord;

out mat3 TBN;
void main(void) {
	// TODO: See Environment Mapping

	//--------------------------------------------------------------------------
	// Determine the reflection direction based on the current camera position
	// and the surface normal. First the vertex position, camera position, and
	// surface normals are transformed into the same coordinate space.
	//--------------------------------------------------------------------------

	vec4 vPosition = vec4(position, 1.0f);
	vec4 cPosition = vec4(cameraPosition, 1.0f);

	for (int i = 0; i < LIGHT_COUNT; ++i) {
        vec4 lightPosition = vec4(lights[i].position, 1.0f);
        interp_LightPosition[i] = vec3(modelViewMatrix * lightPosition);
    }

	interp_VertexPosition = vec3(modelViewMatrix * vPosition);       
	interp_Normal = normalize(mat3(normalMatrix) * normal);
	interp_Texcoord = textureCoordinate.xy;

	vec3 n = interp_Normal;
	vec3 t = normalize(mat3(normalMatrix) * vec3(tangent.xyz));
	vec3 b = cross(n, t) * tangent.w;
	TBN = transpose(mat3(t, b, n));

	
	
	//--------------------------------------------------------------------------
	// The view direction is based on the vector from the camera position to the
	// current vertex. Since we need to evaluate the reflection across the
	// surface of the object, the reflection direction will be the vector
	// that is interpolated across the surface. Then each interpolated 
	// reflection direction will be used as a 3D texture coordinate to determine
	// which part of the cubemap is reflected by the surface. 
	//--------------------------------------------------------------------------
	vec3 cameraPosition = vec3(modelViewMatrix * cPosition);
	vec3 view = normalize(interp_VertexPosition - cameraPosition);
	interp_ReflectDir = reflect(view, interp_Normal);
	
	//--------------------------------------------------------------------------
	// Transform and project the current vertex to the screen.
	//--------------------------------------------------------------------------
	gl_Position = projectionMatrix * modelViewMatrix * vPosition;
}
