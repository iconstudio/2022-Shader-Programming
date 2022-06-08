#version 450
uniform sampler2D u_Texture;
in vec2 vTexCoord;

vec4 plain(sampler2D tex, vec2 coords);
vec4 flip(sampler2D tex, vec2 coords);
vec4 doubleflip1(sampler2D tex, vec2 coords);
vec4 doubleflip2(sampler2D tex, vec2 coords);

out vec4 FragColor;
void main()
{
	FragColor = doubleflip1(u_Texture, vTexCoord);
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

vec4 doubleflip1(sampler2D tex, vec2 coords)
{
	vec4 result = vec4(0.0f);
	
	vec2 coord_flipped = vec2(coords.x, 1.0f - coords.y * 2);
	result = texture(u_Texture, coord_flipped);

	return result;
}
