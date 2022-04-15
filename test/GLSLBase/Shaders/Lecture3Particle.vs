#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in vec3 a_EmitTime;
in vec3 a_Duration;

uniform float u_Time;
uniform vec3 u_Acceleration;

float sqr(float value)
{
	return value * value;
}

void main()
{
	vec3 out_position;

	float t = u_Time;
	float sqr_t = sqr(u_Time);
	out_position = a_Position + t * a_Velocity + 0.5f * u_Acceleration * sqr_t;

	gl_Position = vec4(out_position, 1);
}
