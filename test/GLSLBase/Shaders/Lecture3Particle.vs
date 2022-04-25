#version 450

in vec3 a_Position;
in vec3 a_Velocity;
in float a_EmitTime;
in float a_Duration;
in float a_CurveAmplify;
in float a_CurvePeriod;
in float a_Sphere;

uniform float u_Time;
uniform vec3 u_Acceleration;
uniform bool u_Loop;

const float g_PI = 3.141592;
const mat3 matrix_quaternion = mat3(0f, -1f, 0f, 1f, 0f, 0f, 0f, 0f, 0f);

float sqr(float value)
{
	return value * value;
}

void main()
{ 
	// 원의 외곽선을 따라 생성
	vec3 out_position = a_Position + 0.5f * vec3(cos(a_Sphere * 2f * g_PI), sin(a_Sphere * 2f * g_PI), 0f);
	
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
		out_position += t * a_Velocity + 0.5f * u_Acceleration * sqr_t;

		vec3 direction = normalize(a_Velocity * matrix_quaternion);
		out_position += t * direction * a_CurveAmplify * sin(a_CurvePeriod * t * 2.0f * g_PI);
		out_position.z = 0f;
	}

	gl_Position = vec4(out_position, 1);
}
