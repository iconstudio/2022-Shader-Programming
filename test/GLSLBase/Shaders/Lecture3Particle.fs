#version 450

layout(location=0) out vec4 FragColor;

in vec4 v_Colour;
in vec2 v_TexCoord;

//uniform vec4 u_Color = vec4(1.0f);

void main()
{
	float dist = distance(vec2(0.0f), v_TexCoord);
	if (dist < 0.05f)
	{
		//FragColor = v_Colour;
	} 
	else
	{
		//FragColor = vec4(0.0f);
	}

	//float alpha_basis = FragColor.a;
	//float color_basis = FragColor.rgb;

	FragColor = v_Colour * vec4(1.0f - smoothstep(0.05f, 0.1f, dist));
}
