#version 130
in vec2 v_textureCoords;
in vec4 v_color;
in vec4 v_position;

uniform sampler2D u_colorMap;
uniform sampler2D u_depthMap;

uniform int u_textureWidth;
uniform int u_textureHeight;

out vec4 fragment_color;

void main(void)
{
	vec4 color = texture2D( u_colorMap , v_textureCoords );
	vec4 depth = texture2D( u_depthMap , v_textureCoords );

	fragment_color = vec4( (v_color * color).rgb , 1.0 );
}


