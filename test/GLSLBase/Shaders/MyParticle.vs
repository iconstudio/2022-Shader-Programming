#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_Duration;

uniform float u_Time;
uniform vec3 u_Acceleration;

float sqr(float value)
{
	return value * value;
}

void main()
{
	float t = u_Time - a_EmitTime;

	vec3 out_position = vec3(-100f, -100f, -100f);
	if (0 < t)
	{
		float sqr_t = sqr(t);
		out_position = a_Position + t * a_Velocity + 0.5f * u_Acceleration * sqr_t;
	}

	gl_Position = vec4(out_position, 1);
}
