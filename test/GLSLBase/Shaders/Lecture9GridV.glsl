#version 450
uniform float u_Time;

in vec3 a_Position;
out vec4 v_Colour;

const float PI = 3.141592f;

void main()
{
	int polar_count = 2;

	float dist_x = a_Position.x + 0.5f;
	float dist_r = distance(a_Position.xy, vec2(-0.5f, 0.0f));
	float height = sin(dist_x * PI * polar_count - u_Time);

	float pos_x = dist_x * (dist_r * height) * 0.5f;
	float pos_y = dist_x * (dist_r * height) * 0.5f;

	vec3 v_Position = a_Position + vec3(pos_x, pos_y, 0.0f);
	gl_Position = vec4(v_Position, 1);

	// 들어간 곳은 어둡게
	v_Colour = vec4((height + 1.0f) * 0.5f);
}
