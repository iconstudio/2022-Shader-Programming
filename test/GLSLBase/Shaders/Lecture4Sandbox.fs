#version 450

layout(location=0) out vec4 FragColor;

varying vec3 v_Position;

in vec4 v_Colour;

void main()
{
	vec3 circle_surface = v_Position - vec3(0.5f);
	float distance = length(circle_surface);

	float radius = 0.4f;
	float smoothness = 0.02f;

	vec4 circle = vec4(vec3(smoothstep(radius, radius - smoothness, 0.39f)), 1.0f);
	FragColor = v_Colour * circle;
}
