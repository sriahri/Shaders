#version 410 core

/* Textures for the primary material */
uniform sampler2D diffuseTexture0;
uniform sampler2D normalTexture0;
uniform sampler2D specularTexture0;

/* Textures for the secondary material */
uniform sampler2D diffuseTexture1;
uniform sampler2D normalTexture1;
uniform sampler2D specularTexture1;

/* Interpolated data for Phong shading */
in vec3 interp_Normal;
in vec3 interp_VertexPosition;
in vec3 interp_LightPosition;
in vec2 interp_TextureCoord;

/* TBN Matrix for transforming light direction to tangent space. */
in mat3 TBN;

/* Final Fragment Color */
out vec4 fragColor;

/* Texture Blending */
void main(void) {
	//--------------------------------------------------------------------------
	// Use the alpha mask provided by the second texture (diffuse1) as the
	// mask for the texture blending. This means that the base texture will
	// be solid, but the second texture will be applied over it (depending on
	// the transparency provided by the second texture). 
	//--------------------------------------------------------------------------
    vec4 tex0 = texture2D(diffuseTexture0, interp_TextureCoord);
	vec4 tex1 = texture2D(diffuseTexture1, interp_TextureCoord);
	float alpha = tex1.a;
	vec4 baseMix = mix(tex0, tex1, alpha);
	
	//--------------------------------------------------------------------------
	// Evaluate the normal for the current fragment based on the average of
	// the surface normals defined within each normal map. This is accomplished
	// by looking up the surface normal from each normal map. Then the two 
	// surface normals are added together and then divided by two to get the
	// average.
	//--------------------------------------------------------------------------
	vec3 fragmentNormal0 = normalize(texture2D(normalTexture0, interp_TextureCoord).xyz * 2.0 - 1.0);
	vec3 fragmentNormal1 = normalize(texture2D(normalTexture1, interp_TextureCoord).xyz * 2.0 - 1.0);
	vec3 pixel_normal = normalize(mix(fragmentNormal0, fragmentNormal1, alpha));

	//--------------------------------------------------------------------------
	// Light vectors based on the tangent space normal and light direction.
	// The specular component is based on the surface normal instead of the 
	// normal provided by the normal map due to the different effect provided
	// by each formulation. If the surface normal is used then the specular
	// component will be much brighter and less noisy than using the normal map
	// defined surface normal.
	//--------------------------------------------------------------------------
	vec3 l = normalize(interp_LightPosition - interp_VertexPosition);
	vec3 c = normalize(-interp_VertexPosition);
	vec3 r = normalize(-reflect(l, interp_Normal));
	
	vec3 pixel_light = TBN * l;

	vec4 Ia = vec4(0.5f, 0.5f, 0.5f, 1.0f); 
	vec4 Id = vec4(0.9f, 0.9f, 0.9f, 1.0f); 
	vec4 Is = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	vec4 Ka = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 Kd = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 Ks = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess = 16.0f;
	
	vec4 Iambient = vec4(0.0f);
	vec4 Idiffuse = vec4(0.0f);
	vec4 Ispecular = vec4(0.0f);

	//--------------------------------------------------------------------------
	// Ambient Component of the ADS light model.
	//--------------------------------------------------------------------------
	Iambient = Ia * Ka * baseMix;
 
	//--------------------------------------------------------------------------
	// Diffuse component of the ADS light model. The blended textures that now
	// form the 'baseMix' are now used as the diffuse color of the material 
	// before the ADS light model is applied.
	//--------------------------------------------------------------------------
	float lambertComponent = max(0.0f, dot(pixel_normal, pixel_light)); 
	Idiffuse = baseMix;
	Idiffuse *= (Id * Kd) * lambertComponent;
	
	//--------------------------------------------------------------------------
	// The specular component of the ADS model is modified using the alpha
	// provided from the secondary texture (to ensure that the specular
	// component from each material is accurately represented).
	//--------------------------------------------------------------------------
	Ispecular = (Is * Ks) * pow(max(dot(r, c), 0.0f), shininess);
	vec4 specular0 = texture2D(specularTexture0, interp_TextureCoord);
	vec4 specular1 = texture2D(specularTexture1, interp_TextureCoord);
	vec4 specMix = mix(specular0, specular1, alpha);
	Ispecular *= specMix;

	fragColor = Iambient + Idiffuse + Ispecular;  
}
