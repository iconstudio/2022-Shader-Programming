#version 450
uniform sampler2D u_Texture;
in vec2 vTexCoord;

out vec4 FragColor;
void main()
{
	FragColor = texture(u_Texture, vTexCoord);
}
