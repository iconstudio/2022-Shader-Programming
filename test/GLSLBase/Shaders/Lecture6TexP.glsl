#version 450
uniform sampler2D u_Texture;
uniform sampler2D u_TexRGB;
uniform sampler2D u_TexChecker;
uniform float u_Time;

in vec2 vTexCoord;

vec4 plain(sampler2D tex, vec2 coords);
vec4 flip(sampler2D tex, vec2 coords);
vec4 doubleflip_unoptimized(sampler2D tex, vec2 coords);
vec4 doubleflip(sampler2D tex, vec2 coords);
vec4 rearrange_triple_reversed(sampler2D tex, vec2 coords);
vec4 rearrange_triple(sampler2D tex, vec2 coords);
vec4 rearrange_tall_reversed(sampler2D tex, vec2 coords);
vec4 pattern_brick(sampler2D tex, vec2 coords);
vec4 pattern_brick_n(sampler2D tex, vec2 coords, int h, int v);
vec4 half_placer(sampler2D tex1, sampler2D tex2, vec2 coords);
vec4 stencil_basic(sampler2D tex1, sampler2D tex2, vec2 coords);
vec4 stencil_basic_by2(sampler2D tex1, sampler2D tex2, vec2 coords1, vec2 coords2);
vec4 stencil_moving(sampler2D tex1, sampler2D tex2, vec2 coords);

const float PI = 3.141592;

out vec4 FragColor;
void main()
{
	//FragColor = pattern_brick(u_Texture, vTexCoord);
	//FragColor = pattern_brick_n(u_Texture, vTexCoord, 4, 4);
	FragColor = stencil_moving(u_TexRGB, u_TexChecker, vTexCoord);
}

vec4 plain(sampler2D tex, vec2 coords)
{
	return texture(tex, vTexCoord);
}

vec4 flip(sampler2D tex, vec2 coords)
{
	vec4 result = vec4(0.0f);
	
	vec2 coord_flipped = vec2(coords.x, 1.0f - coords.y);
	result = texture(tex, coord_flipped);

	return result;
}

vec4 doubleflip_unoptimized(sampler2D tex, vec2 coords)
{
	vec4 result = vec4(0.0f);
	
	if (coords.y < 0.5f)
	{
		vec2 not_flipped = vec2(coords.x, 1.0f - coords.y * 2.0f);
		result += texture(tex, not_flipped);
	}
	else
	{
		vec2 coord_flipped = vec2(coords.x, (coords.y) * 2.0f);
		result += texture(tex, coord_flipped);
	}

	return result;
}

vec4 doubleflip(sampler2D tex, vec2 coords)
{
	float y_flipped = abs(2.0f * (coords.y - 0.5f));

	return texture(tex, vec2(coords.x, y_flipped));
}

vec4 rearrange_triple_reversed(sampler2D tex, vec2 coords)
{
	float x_morphed = fract(coords.x * 3.0f);
	float y_morphed = coords.y / 3 + floor(coords.x * 3.0f) / 3.0f;

	return texture(tex, vec2(x_morphed, y_morphed));
}

vec4 rearrange_triple(sampler2D tex, vec2 coords)
{
	float x_morphed = fract((coords.x) * 3.0f);
	float y_morphed = coords.y / 3 + floor((1.0f - coords.x) * 3.0f) / 3.0f;

	return texture(tex, vec2(x_morphed, y_morphed));
}

// 
vec4 rearrange_tall_reversed(sampler2D tex, vec2 coords)
{
	float x_morphed = coords.x;
	float y_morphed = fract(coords.x / 3) + floor((1.0f - coords.y) * 3.0f) / 3.0f;

	return texture(tex, vec2(x_morphed, y_morphed));
}

vec4 pattern_brick(sampler2D tex, vec2 coords) 
{
	float x_morphed = fract(coords.x * 2.0f) + floor(coords.y * 2.0f) * 0.5f;
	float y_morphed = fract(coords.y * 2.0f);

	return texture(tex, vec2(x_morphed, y_morphed));
}

// ½ÃÇè!
vec4 pattern_brick_n(sampler2D tex, vec2 coords, int h, int v)
{
	float tex_u = coords.x * (h);
	float tex_v = coords.y * (v);

	float x_morphed = fract(tex_u) + floor(tex_v) * 0.5f;
	float y_morphed = fract(tex_v);

	return texture(tex, vec2(x_morphed, y_morphed));
}

vec4 half_placer(sampler2D tex1, sampler2D tex2, vec2 coords)
{
	float x_morphed = fract(coords.x * 2.0f);
	float y_morphed = coords.y;

	if (coords.x <= 0.5f)
	{
		return texture(tex1, vec2(x_morphed, y_morphed));
	}
	else
	{
		return texture(tex2, vec2(x_morphed, y_morphed));
	}
}

vec4 stencil_basic(sampler2D tex1, sampler2D tex2, vec2 coords)
{
	return texture(tex1, coords) * texture(tex2, coords);
}

vec4 stencil_basic_by2(sampler2D tex1, sampler2D tex2, vec2 coords1, vec2 coords2)
{
	return texture(tex1, coords1) * texture(tex2, coords2);
}

vec4 stencil_moving(sampler2D tex1, sampler2D tex2, vec2 coords)
{
	float x_morphed = coords.x + u_Time;
	x_morphed = fract(x_morphed);

	float y_morphed = coords.y;

	return stencil_basic_by2(tex1, tex2, coords, vec2(x_morphed, y_morphed));
}
