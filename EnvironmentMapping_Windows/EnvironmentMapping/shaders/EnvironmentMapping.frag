#version 410 core

/* 
 * Cubemap sampler (6) images composed into a 3D texture. The cubemap sampler
 * is a special case that is implemented by OpenGL to provide the set of cube
 * images that can be used as the environment map. Unlike simple 2D textures
 * which require a 2D (u, v) texture coordinate, the cubemap requires a 3D
 * texture coordinate which will be mapped onto the surface of the cube.
 */
uniform samplerCube cubeMap;

/*
 * The reflection direction based on the viewing angle of the camera and the
 * surface normal. This vector has been interpolated across the surface to 
 * define which parts of the environment are reflected towards the camera.
 */
in vec3 interp_ReflectDir;

/* Resulting fragment color */
out vec4 fragColor;

void main(void) {
	//--------------------------------------------------------------------------
	// Cubemap texture coordinates are determined by the view dependent
	// reflection direction. This direction represents the part of the 
	// environment map that will be reflected off of the objects surface to be
	// viewed by the camera.
	//--------------------------------------------------------------------------
	vec4 cubeMapColor = texture(cubeMap, interp_ReflectDir);
	
	//--------------------------------------------------------------------------
	// The material property can be changed to modify the color of the reflected
	// environment map. These values can be changed to make the object appear
	// to reflective while also making the object appear a specific color.
	//--------------------------------------------------------------------------
	vec4 Kd = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	//--------------------------------------------------------------------------
	// The reflection factor determines the ratio of the mixture between the 
	// material property Kd and the cube map reflection color value. Internally
	// the mix function is simply doing a fractional component for each of the
	// two provided vectors: 
	//
	//   fragColor = (1.0 - reflectFactor) * Kd + reflectFactor * cubeMapColor
	//
	// If reflectFactor is set to 1.0, then the cubeMapColor will be used
	// If reflectFactor is set to 0.0, then the Kd color will be used
	//--------------------------------------------------------------------------
	float reflectFactor = 1.0f;
	fragColor = mix(Kd, cubeMapColor, reflectFactor);
}
