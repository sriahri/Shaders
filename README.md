# Shaders
The shaders are a core component of Computer Graphics and continuously revolutionize all forms of graphics, physical simulation, visualization, etc.
## PassThrough Shader
The pass-through shader serves as a basic “Hello World” of the programmable graphics pipeline. This shader doesn’t alter the appearance of the loaded 3D objects but rather passes their geometric and preset color information (a predefined or hard coded color) directly through the stages of the pipeline, minimizing the amount of shader code required to complete the rendering process. The primary purpose of a pass-through shader is to maintain the integrity of the object’s surface characteristics without introducing any modifications. In the case of rendering the final output, the pass-through shader ensures that the loaded object is displayed in a solid color, preserving the inherent colors and attributes defined by the original mesh and hard-coded surface color.
![PassThrough Shader](https://github.com/sriahri/Shaders/blob/main/Results/Passthrough_shader.png)

## Color Mapping Shader
The color-mapping shader provides a basic demonstration of how the surface of an object can be shaded using a linear interpolation of colors based on a given surface normal **_n^_**
 provided by the geometry of the loaded mesh. The surface normal components n^=(x,y,z)
 are directly mapped to the color components (R,G,B)
 of the fragment color. The resulting multi-color blend is due to the orientation of the surface normals about the y^
 axis. This process defines the overall operation of the shader as a basic color mapping.
![ColorMapping Shader](https://github.com/sriahri/Shaders/blob/main/Results/ColorMapping_Shader.png)
## The Ambient-Diffuse-Specular (ADS) Lighting Model
The most common illumination model that has persisted through the development of computer graphics over the course of several years is based on a three-component form that is computationally efficient and easy to implement using modern shader languages. This lighting model has become known as the Ambient, Diffuse, Specular or (ADS) light shading model.
The ADS lighting model provides the basis for understanding and implementing the simplest form of basic light effects. There are different methods for implementing this lighting model which impact both the performance and visual quality of the result. This includes both Gouraud and Phong lighting models that use the underlying ideas.
### Gouraud Shading: 
For each vertex, the light intensity provided by the light source is evaluated within the object surface using the vertex normal. Once this intensity value is determined for each vertex, the intensity values are linearly interpolated between vertices (color interpolation).
![Gouraud Shading](https://github.com/sriahri/Shaders/blob/main/Results/Gouraud_Shader.png)
### Phong Shading: 
For each fragment, the surface normal is linearly interpolated based on the position of the fragment in the surface and each of the vertex normals connected to the current face. This will determine the light intensity value for each fragment in the surface (normal interpolation).
![Phong Shading](https://github.com/sriahri/Shaders/blob/main/Results/Phong_Shader.png)

