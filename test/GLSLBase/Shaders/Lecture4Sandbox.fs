#version 450

layout(location=0) out vec4 FragColor;

in vec3 a_Position;
in vec3 a_Colour;

in vec3 v_Position;
in vec4 v_Colour;
in vec4 v_ColourOverride;
in vec2 v_TexCoord;

uniform vec3 u_Points[10];
uniform float u_Time = 0.0f;

const float g_PI = 3.141592f;

vec4 CrossPattern();
vec4 DrawEdgedCircle(vec2 pos, float outer_radius, float thickness);
vec4 DrawDuplicatedCircle(vec2 centre, vec2 pos, int count);
vec4 DrawCircle(vec2 point, vec2 centre);
vec4 DrawCircles(vec2 centre);
vec4 DrawRaderCirle(vec2 centre, vec2 pos);
vec4 DrawRaderCirle2(vec2 centre, vec2 pos);
vec4 DrawSegmentedLine(vec2 centre, vec2 pos);

void main()
{
	//float radius = 0.4f;
	//float smoothness = 0.02f;
	//circle_color = vec4(vec3(smoothstep(radius, radius - smoothness, 0.39f)), 1.0f);
	//FragColor = v_Colour * circle_color;

	//FragColor = DrawEdgedCircle(v_Position.xy, 0.4f, 0.02f);
	//FragColor = CrossPattern();
	//FragColor = DrawDuplicatedCircle(v_Colour.xy, 10, 0.0f);
	//FragColor = DrawCircle(u_Points, v_Colour.xy);

	//FragColor = DrawCircles(v_Colour.xy);
	//FragColor = DrawCircle(u_Points[1].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[2].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[3].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[4].xy , v_Colour.xy);

	FragColor = DrawRaderCirle2(vec2(0.5f, 0.0f), v_Colour.xy);
	//FragColor *= v_Colour;
}

vec4 CrossPattern()
{
	int count = 10;
	float interpolation = g_PI * 0.5f;

	float xaxis = sin(count * (v_Colour.x * 2.0f * g_PI) + interpolation);
	float yaxis = sin(count * (v_Colour.y * 2.0f * g_PI) + interpolation);
	return vec4(max(xaxis, yaxis));
}

vec4 DrawEdgedCircle(vec2 pos, float outer_radius, float thickness)
{
	float inner_radius = outer_radius - thickness;

	float radius = distance(pos, vec2(0.0f));

	return vec4(vec3(smoothstep(inner_radius, outer_radius, radius)), 1.0f);
}

vec4 DrawDuplicatedCircle(vec2 centre, vec2 pos, int count)
{
	float interpolation = g_PI * 0.5f;
	float dist = distance(pos, centre);
	return vec4(sin(count * (dist * 4 * g_PI))); // 시험!!!!!
}

vec4 DrawCircle(vec2 point, vec2 centre)
{
	float dist = distance(point, centre);
	if (dist < 0.4f)
	{
		return vec4(1.0f);
	}
	else
	{
		return vec4(0.0f);
	}
}

// 시험!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 제대로 된 물결 파동을 만들어라!!!!!!!!!!!!!!!
// 1. 안쪽 파동은 일정 시간 이후에는 나오지 않게 한다.
// 2. 나오는 파동은 점점 약해진다.
// 3. 나오는 파동의 속도는 점점 느려진다.
// 4. 
vec4 DrawCircles(vec2 centre)
{
	vec4 result = vec4(0.0f);
	for (int i = 0; i < 10; i++)
	{
		float dist = distance(u_Points[i].xy, centre);
		if (dist < u_Time)
		{
			result += vec4(sin(-u_Time * 100 + 10 * (dist * 4 * g_PI)));
			//DrawDuplicatedCircle(u_Points[i].xy, centre, 10);
			//vec4(1.0f);
		}
	}
	return result;
}

// 시험!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 더 멋지게 튜닝해봐라
vec4 DrawRaderCirle(vec2 centre, vec2 pos)
{
	float rader_dist = distance(centre, pos);
	float rader_factor = (rader_dist * 2 * g_PI) - u_Time * 10;

	int count = 2;
	float rader_sin_value = sin(count * rader_factor);
	int rader_range = 4;

	float wave = clamp(pow(rader_sin_value, rader_range), 0.0f, 1.0f);

	//vec4 result = vec4(wave);
	float magnificent = 2.0f;
	//result /= result;

	for (int i = 0; i < 10; i++)
	{
		float dist = distance(u_Points[i].xy, pos);
		if (dist < 0.1f)
		{
			//float greener = 10 * rader_sin_value * (0.1f - dist) * magnificent;
			//result += vec4(0, magnificent, 0, 0);
		}
		float factor = (dist * 2 * g_PI);
		float sin_value = sin(count * factor);
		sin_value = clamp(sin_value, 0.0f, 1.0f);
		
		//result += vec4(sin_value * 0.2f);
		wave += (sin_value * 0.2f);
	}

	//return result;
	return vec4(wave);
}

vec4 DrawRaderCirle2(vec2 centre, vec2 pos)
{
	float rader_dist = distance(centre, pos);
	float rader_factor = (rader_dist * 2 * g_PI) - u_Time * 10;

	int count = 2;
	int rader_range = 4;

	float rader_sin_value = sin(count * rader_factor);
	float wave = clamp(pow(rader_sin_value, rader_range), 0.0f, 1.0f);

	float magnificent = 2.0f;
	//wave /= magnificent;
	
	vec4 result = vec4(wave);

	for (int i = 0; i < 10; i++)
	{
		float dist = distance(u_Points[i].xy, pos);
		if (dist < 0.1f)
		{
			float greener = 10 * rader_sin_value * (0.1f - dist) * magnificent;
			float alpha = result.a;

			result += vec4(0, greener, 0, max(alpha, (greener + alpha) * (0.1f - dist)));
		}

		//float factor = (dist * 4 * g_PI);
		//float sin_value = sin(count * factor);
		//sin_value = clamp(sin_value, 0.0f, 1.0f);
		
		//result += vec4(sin_value * 0.1f);
	}

	return result;
}
