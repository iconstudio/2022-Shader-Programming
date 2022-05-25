#version 450

in vec3 a_Position;

out vec3 v_Position;

void main()
{
	gl_Position = vec4(a_Position, 1);
}
