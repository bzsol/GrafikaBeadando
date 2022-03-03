#version 430 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader
uniform float vonalY;

void main()
{
    gl_Position = aPos;
    vertexColor = vec4(0.0, 0.0, 1.0, 1.0);
}