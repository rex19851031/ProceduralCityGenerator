#version 130
in vec2 a_textureCoords;
in vec3 a_position;
in vec4 a_color; 
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_biTangent;

out vec2 v_textureCoords;
out vec4 v_color;
out vec3 v_position;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_biTangent;

uniform float u_time;
uniform mat4 u_worldToClipMatrix;
uniform mat4 u_objectToWorldMatrix;

void main(void)
{
	v_normal = a_normal;
	v_tangent = a_tangent;
	v_biTangent = a_biTangent;

	v_position = (u_objectToWorldMatrix * vec4(a_position.xyz,1.0)).xyz;
	v_color = a_color;
	v_textureCoords = a_textureCoords;

	gl_Position = u_worldToClipMatrix * u_objectToWorldMatrix * vec4(a_position,1.0);
}