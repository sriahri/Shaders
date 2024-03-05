#version 410 core

/* Cubemap sampler (6) images composed into a 3D texture */
uniform samplerCube cubeMap;

/* 
 * Interpolated light, normal, and fragment position. This is the same as the 
 * light direction specified within the Phong lighting model.
 */
in vec3 interp_Normal;
in vec3 interp_VertexPosition;
in vec3 interp_LightPosition;

/*
 * The reflection direction represents the reflection of the environment cube
 * shown on the surface of the object with respect to the current viewing
 * direction. This value is calculated in the vertex shader because the
 * coordinate transformation (based on the the model view matrix) and since the 
 * vertex shader will be executed less frequently than the fragment shader, this
 * provides a more efficient implementation because the interpolation process
 * is more efficient.
 */
in vec3 interp_ReflectDir;

/* Resulting fragment color */
out vec4 fragColor;

/* Environment Mapped Material */
void main(void) {
	//--------------------------------------------------------------------------
	// Determine the environment map fragment color based on the reflection
	// direction.
	//--------------------------------------------------------------------------
	vec4 cubeMapColor = texture(cubeMap, interp_ReflectDir);

	//--------------------------------------------------------------------------
	// Light vector calculations from Phong shading. These are used to apply
	// the same ADS light model to the environment mapped object.
	//--------------------------------------------------------------------------
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	//--------------------------------------------------------------------------
	// The color of the light is assumed to be white.
	//--------------------------------------------------------------------------
	vec4 Ia = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Id = vec4(1.0f, 1.0f, 1.0f, 1.0f); 
	vec4 Is = vec4(1.0f, 0.956f, 0.612f, 1.0f);
	
	//--------------------------------------------------------------------------
	// The material properties are modified to presesnt a brass finish based
	// on the reflectance of the surface and the modified color values of the 
	// surface material.
	//--------------------------------------------------------------------------
	vec4 Ka = vec4(0.645f, 0.45f, 0.21f, 1.0f); 
	vec4 Kd = vec4(0.876f, 0.656f, 0.212f, 1.0f); 
	vec4 Ks = vec4(1.0f, 0.956f, 0.612f, 1.0f);
	float shininess = 16.0f;
	
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
	Iambient = cubeMapColor * Ia * Ka;
	float lambertComponent = max(0.0f, dot(interp_Normal, l));
	Idiffuse = cubeMapColor;
	Idiffuse *= (Id * Kd) * lambertComponent; 
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess); 
	
	fragColor = Iambient + Idiffuse + Ispecular;
}
