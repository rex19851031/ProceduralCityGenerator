#version 130
in vec2 v_textureCoords;
in vec4 v_color;
in vec4 v_position;

uniform sampler2D u_diffuseMap;

out vec4 fragment_color;

void main()
{ 
	float intensity = dot( texture2D(u_diffuseMap,v_textureCoords).rgb , vec3(0.33,0.33,0.33) );
	intensity = clamp( intensity - 0.25 , 0.0 , 0.1 ) * 10;
	fragment_color = vec4(vec3(1.0,1.0,1.0) * intensity , 1.0);
}