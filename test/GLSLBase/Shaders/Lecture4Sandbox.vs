#version 450
attribute vec3 a_Position;

out vec3 v_Position;
out vec4 v_Colour; // out

void main()
{
	vec3 my_color = a_Position + 0.0f; 
	v_Colour = vec4(my_color, 1f);
	v_Colour.z = 0f;

	v_Position = a_Position;

	gl_Position = vec4(a_Position, 1);
}
