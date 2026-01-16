# [RasterTek Tutorials](https://www.rastertek.com/tutdx11win10.html)

- # 04. Buffers, Shaders & HLSL
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/2tris-vertex-interpolation.png)
|:--:|
*Two triangles forming a rectangle. Vertex color interpolation.*
*The gradient doesnt interpolate properly since the colors can't interpolate across different triangles.*

- # 05. Textures
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/linear.gif)
|:--:|
*Texture mapped to 2 triangles, camera movement, linear filtering.*

- # 06. Diffuse Lighting
![Image](https://raw.githubusercontent.com/meursw/Graphics-Programming/refs/heads/main/DirectX%2011/Results/diffuse_lighting.gif)
|:--:|
*Diffuse lighting on a triangle.*

- # 07. 3D Model Rendering
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/drybonestex.gif)
|:--:|
*3D model rendering. Wrote .obj file parser to load a 3D model I made in Blender 4.0.* 

- # 08. Scaling, Rotation & Translation
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/2drybones.gif)
|:--:|
*Two of the same model with different transforms.*
*The left model had its translation applied first, so now its rotating around the origin.*

- # 09. Ambient Lighting
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/ambient%2Bdiffuse.png)
|:--:|
*Ambient and Diffuse Lighting. Light positioned at (-1,0,0).*

- # 10. Specular Lighting
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/Specular%20Lighting.png)
|:--:|
*Specular Lighting* 
*Diffuse Color is red and Specular Color is green. Specular power set to 8.*

- # 11. Multiple Point Lights
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/pointlight-attenuation.gif)
|:--:|
Four points lights with oscillating attenuation
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/spotlight-attenuation.gif)
Four spot lights with oscillating attenuation

- # 12. 2D Rendering 
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/2dimages.gif)
|:--:|
2D Rendering. Also showcasing custom made camera movement.

- # 13. Sprites & Timers
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/2dsprites.gif)
|:--:|
2D Animated Sprites. 93 frames, 20ms per frame.

- # 14-15. Font Engine & FPS Counters
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/fpscounter.gif)
|:--:|
Rendering an FPS counter by rendering text using a font texture.
Rendering 10.000 models. As we can see, the GPU's frustum culling isn't enough to optimize this scene.
In later chapters, we will implement CPU side frustum culling.

- # 16. Direct Input
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/directinput.gif)
|:--:|
Using Direct Input 8.
Rendering keyboard input to the screen.

- # 17. Multitexturing
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/multitexturing2tex.png)
|:--:|
Multitexturing.
Rendering a model with 2 textuers and blending them together.

- # 18-19. Light Maps & Alpha Mapping
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/lightandalphamaps.png)
|:--:|
Using light maps to avoid lighting calculation on the left.
Blending between two textures with an alpha map on the right.

- # 20. Normal mapping
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/normalmaps.gif)
|:--:|
Normal mapping on a plane, a cube and a sphere.
Tangent and bitangent vectors for the mesh are calculated on the CPU.

- # 21. Specular mapping
![Image](https://github.com/meursw/Graphics-Programming/blob/main/DirectX%2011/Results/specmapping.gif)
|:--:|
Specular mapping with normal mapping.



