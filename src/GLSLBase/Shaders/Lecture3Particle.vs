#version 450
in vec3 a_Position;
//in vec4 a_Color;
in vec3 a_Velocity;

uniform float u_Time;

out vec4 v_Color;

void main()
{
	vec3 out_position;
	out_position = a_Position + u_Time * a_Velocity;

	gl_Position = vec4(out_position, 1);

	//v_Color = a_Color;
}
