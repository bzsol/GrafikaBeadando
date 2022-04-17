#version 430

uniform vec3 myColor;
out vec4 color;

void main(void)
{
	color = vec4(myColor, 1.0);
	color = vec4(1.0, 1.0, 1.0, 1.0);
}