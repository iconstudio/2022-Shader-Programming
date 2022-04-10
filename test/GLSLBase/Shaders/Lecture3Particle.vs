#version 450
in vec3 a_Position;
in vec3 a_Velocity;

uniform float u_Time;

void main()
{
	vec3 out_position;
	out_position = a_Position + u_Time * a_Velocity;

	gl_Position = vec4(out_position, 1);
}
