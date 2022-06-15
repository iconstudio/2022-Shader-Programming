#version 450
attribute vec3 a_Position;

out vec3 v_Position;

uniform float u_Time = 0.0f;

const float PI = 3.141592f;

void main()
{
	// 0~2pi ·Î º¸°£

	int polar_count = 3;

	float interpolation = a_Position.x + 1.0f;
	float height = sin(-u_Time + polar_count * interpolation * PI);
	v_Position = a_Position + vec3(0, height, 0);

	gl_Position = vec4(v_Position, 1);
}
