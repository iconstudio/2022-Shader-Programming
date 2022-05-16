#version 450

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Colour;
in vec2 v_TexCoord;

uniform vec3 u_Points[10];

const float g_PI = 3.141592f;

vec4 CrossPattern();
vec4 DrawEdgedCircle(vec2 pos, float outer_radius, float thickness);
vec4 DrawDuplicatedCircle(vec2 pos, int count, float range);
vec4 DrawCircle(vec3 point);

void main()
{
	vec2 circle_surface = v_Position.xy;
	float radius = length(circle_surface);

	// float dist = distance(v_Position.xy, vec(0.5f));

	vec4 circle_color = vec4(0.0f);
	if (radius < 0.4f)
	{
		circle_color = vec4(1.0f);
	}

	//float radius = 0.4f;
	//float smoothness = 0.02f;
	//circle_color = vec4(vec3(smoothstep(radius, radius - smoothness, 0.39f)), 1.0f);

	//circle_color = DrawEdgedCircle(v_Position.xy, 0.4f, 0.02f);
	//FragColor = v_Colour * circle_color;

	//FragColor = CrossPattern();

	//FragColor = DrawDuplicatedCircle(v_Colour.xy, 10, 0.0f);

	FragColor = DrawCircle(u_Points, v_Colour.xy);
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

vec4 DrawDuplicatedCircle(vec2 pos, int count, float range)
{
	float interpolation = g_PI * 0.5f;
	float dist = distance(pos, vec2(range));
	return vec4(sin(count * (dist * 4 * g_PI))); // ½ÃÇè!!!!!
}

vec4 DrawCircle(vec2 point, vec2 centre)
{
	float dist = distance(point, centre);
	vec4 circle_color = vec4(0.0f);
	if (radius < 0.4f)
	{
		return vec4(1.0f);
	}
	else
	{
		return vec4(0.0f);
	}
}
