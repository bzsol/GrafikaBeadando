#version 330 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader
uniform float vonalY;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y + (2.0f * Y), aPos.z, 1.0);
    vertexColor = vec4(0.0, 0.0, 1.0, 1.0); // set the output variable to a dark-red color
}