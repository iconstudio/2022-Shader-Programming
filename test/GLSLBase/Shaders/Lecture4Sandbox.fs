#version 450

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Colour;
in vec2 v_TexCoord;

uniform vec3 u_Points[10];
uniform float u_Time = 0.0f;

const float g_PI = 3.141592f;

vec4 CrossPattern();
vec4 DrawEdgedCircle(vec2 pos, float outer_radius, float thickness);
vec4 DrawDuplicatedCircle(vec2 centre, vec2 pos, int count);
vec4 DrawCircle(vec2 point, vec2 centre);
vec4 DrawCircles(vec2 centre);
vec4 DrawRader();

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

	FragColor = DrawCircles(v_Colour.xy);
	//FragColor = DrawCircle(u_Points[1].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[2].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[3].xy, v_Colour.xy);
	//FragColor += DrawCircle(u_Points[4].xy , v_Colour.xy);
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
// 제대로 된 물결 파동을 만들어라!!!!!!!!!!!!!!
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
