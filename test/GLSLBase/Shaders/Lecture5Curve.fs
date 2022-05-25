#version 450

layout(location=0) out vec4 FragColor;

vec4 DrawSegmentedLine(vec2 centre, vec2 pos);

void main()
{
	FragColor = DrawSegmentedLine(vec2(0, 0), vec2(0.5f));
}

vec4 DrawSegmentedLine(vec2 centre, vec2 pos)
{
	return vec4(1,1,1,1);
}
