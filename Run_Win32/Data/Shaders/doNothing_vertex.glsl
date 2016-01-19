#version 130
in vec2 a_textureCoords;
in vec3 a_position;
in vec4 a_color; 

out vec2 v_textureCoords;
out vec4 v_color;
out vec3 v_position;

uniform float u_time;
uniform mat4 u_worldToClipMatrix;

void main(void)
{
	v_position = a_position;
	v_color = a_color;
	v_textureCoords = a_textureCoords;
	gl_Position = u_worldToClipMatrix * vec4(a_position,1.0);
}