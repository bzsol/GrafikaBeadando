#version 430 core
out vec4 FragColor;  
in vec3 ourColor;
  
void main()
{
	vec2 pos = gl_FragCoord.xy/vec2(600., 600.);
    FragColor = vec4(ourColor, 1.0);
}