#version 430 core
out vec4 FragColor;  
in vec3 ourColor;
in float seged;
  
void main()
{
	vec2 pos = gl_FragCoord.xy/vec2(600., 600.);
	vec2 pos1 = vec2(0.0, 0.5) + vec2(1.0, seged);
    FragColor = vec4(ourColor, 1.0);
}