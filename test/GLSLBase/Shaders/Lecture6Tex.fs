#version 450
uniform sampler2D u_Texture;
in vec2 vTexCoord;

vec4 plain(sampler2D tex, vec2 coords);
vec4 flip(sampler2D tex, vec2 coords);
vec4 doubleflip_unoptimized(sampler2D tex, vec2 coords);
vec4 doubleflip(sampler2D tex, vec2 coords);

const float PI = 3.141592;

out vec4 FragColor;
void main()
{
	FragColor = doubleflip(u_Texture, vTexCoord);
}

vec4 plain(sampler2D tex, vec2 coords)
{
	return texture(u_Texture, vTexCoord);
}

vec4 flip(sampler2D tex, vec2 coords)
{
	vec4 result = vec4(0.0f);
	
	vec2 coord_flipped = vec2(coords.x, 1.0f - coords.y);
	result = texture(u_Texture, coord_flipped);

	return result;
}

vec4 doubleflip_unoptimized(sampler2D tex, vec2 coords)
{
	vec4 result = vec4(0.0f);
	
	if (coords.y < 0.5f)
	{
		vec2 not_flipped = vec2(coords.x, 1.0f - coords.y * 2.0f);
		result += texture(u_Texture, not_flipped);
	}
	else
	{
		vec2 coord_flipped = vec2(coords.x, (coords.y) * 2.0f);
		result += texture(u_Texture, coord_flipped);
	}

	return result;
}

vec4 doubleflip(sampler2D tex, vec2 coords)
{
	float y_flipped = abs(2.0f * (coords.y - 0.5f));

	return texture(u_Texture, vec2(coords.x, y_flipped));
}
