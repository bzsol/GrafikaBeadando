#version 430

out vec4 color;

uniform float colorchanger;

void main(void)
{
	if(colorchanger == 0)
	{
		color = vec4(0, 1, 0, 1.0);
	}
	else if(colorchanger == 1)
	{
		color = vec4(0.294, 0.000, 0.510,1.0);
	}
	else if(colorchanger == 2)
	{
		color = vec4(1, 0, 0,1.0);
	}
}