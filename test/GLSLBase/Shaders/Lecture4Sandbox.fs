#version 450

layout(location=0) out vec4 FragColor;

in vec3 v_Position;
in vec4 v_Colour;
in vec2 v_TexCoord;

vec4 DrawEdgedCircle(float outer_radius, float thickness);

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

	circle_color = DrawEdgedCircle(0.4f, 0.02f);
	FragColor = v_Colour * circle_color;
}

vec4 DrawEdgedCircle(float outer_radius, float thickness)
{
	float inner_radius = outer_radius - thickness;

	float radius = distance(v_Position.xy, vec2(0.0f));

	return vec4(vec3(smoothstep(inner_radius, outer_radius, radius)), 1.0f);
}
