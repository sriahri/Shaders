#version 410 core

/* Cubemap sampler (6) images composed into a 3D texture */
uniform samplerCube cubeMap;

/*
 * The interpolated refelction and refraction directions provide two different
 * 3D texture coordinates: one to look up the reflected color value for this
 * fragment and the other provides the refracted color value for this fragment.
 * These values are then both combined to provide the illusion of a transparent
 * object surface.
 */
in vec3 interp_ReflectDir;
in vec3 interp_RefractDir;

out vec4 fragColor;

/* Refraction Shader */
void main(void) {
	//--------------------------------------------------------------------------
	// Look up both the reflection and refraction color values based on the 
	// same cubemap. The resulting fragment color will be a combination of these
	// color values based on a simple ratio.
	//--------------------------------------------------------------------------
	vec4 reflectColor = texture(cubeMap, interp_ReflectDir);
	vec4 refractColor = texture(cubeMap, interp_RefractDir);

	//--------------------------------------------------------------------------
	// The final result is defined by the relationship between the reflection
	// and refraction ratio (how much of the cubemap is reflected and how much
	// is refracted).
	//--------------------------------------------------------------------------
	float reflectFrac = 0.9f;
	vec4 c = vec4(0.0f);
	c += reflectFrac * refractColor;
	c += (1.0f - reflectFrac) * reflectColor;
	
	fragColor = c;
}
