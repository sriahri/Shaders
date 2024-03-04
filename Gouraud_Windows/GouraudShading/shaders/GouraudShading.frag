#version 410 core

/* 
 * Vector4 that contains the (R,G,B,A) components of the calculated light color.
 * This is the color value that will be interpolated across the object surface.
 */
in vec4 interpShadedColor;
out vec4 color;

/* Gouraud Shading */
void main() {
	color = interpShadedColor;  
}
