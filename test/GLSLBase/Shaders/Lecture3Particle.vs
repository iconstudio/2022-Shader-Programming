#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_Duration;
in float a_CurveAmplify;
in float a_CurvePeriod;

uniform float u_Time;
uniform vec3 u_Acceleration;
uniform bool u_Loop;

const float g_PI = 3.141592;

float sqr(float value)
{
	return value * value;
}

void main()
{
	vec3 out_position = vec3(-100f, -100f, -100f);
	
	float time = (u_Time - a_EmitTime);

	if (0 <= time)
	{
		float t = time;
		if (u_Loop) {
			float normal = time / a_Duration;
			float infra = fract(normal);
		
			t = infra * a_Duration;
		}

		float sqr_t = sqr(t);

		// 일정 주기로, 사인 곡선을 따라 움직이는 입자
		out_position.x = a_Position.x + t * a_Velocity.x + 0.5f * u_Acceleration.x * sqr_t;
		out_position.y = a_Position.y + a_CurveAmplify * sin(a_CurvePeriod * t * 2.0f * g_PI);
		out_position.z = 0f;
	}

	gl_Position = vec4(out_position, 1);
}
