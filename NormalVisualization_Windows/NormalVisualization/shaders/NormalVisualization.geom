#version 410 core

/* 
 * Take in the triangle from the VBO and then generate the line_strip
 * (only containing a single line segment) that represents the surface normal.
 */
layout (triangles) in;			// The type of primitive of the mesh (input)
layout (line_strip) out;		// The type of geometry primitive that will be created
layout (max_vertices = 2) out;	// Each primitive is 2 vertices

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;
uniform float normalScale;

in Vertex {
	vec3 position;
	vec3 normal;
	vec4 tangent;
	vec3 textureCoord;
	vec3 color;
} vertex[];

/*
 * For each vertex of the current triangle, transform the position and emit it
 * as the first vertex. Then for the second vertex (that will form the line of
 * the normal), transform the position plus the normal direction times the 
 * provided normal scale. This will generate a line segment from the position
 * of the vertex in the direction of the vertex normal. The length of the line
 * will be defined by the normal scale. For every triangle, three normals are
 * generated (redundant information in this case).
 */
void main() {
	for ( int i = 0; i < gl_in.length(); i++ ) {
		vec3 p = vertex[i].position;
		vec3 n = vertex[i].normal;
		
		//----------------------------------------------------------------------
		// Generate the position of the vertex
		//----------------------------------------------------------------------
		gl_Position = projectionMatrix * modelViewMatrix * vec4(p, 1.0f);
		EmitVertex(); // Add vertex to the output primitive
		
		//----------------------------------------------------------------------
		// Generate the position of the normal (to form a line segment)
		//----------------------------------------------------------------------
		gl_Position = projectionMatrix * modelViewMatrix * vec4(p + n * normalScale, 1.0f);
		EmitVertex(); // Add vertex to the output primitive
	}

	EndPrimitive(); // Done with the geometry, emit it to the fragment shader
}
