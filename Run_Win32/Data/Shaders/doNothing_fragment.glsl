#version 130
in vec2 v_textureCoords;
in vec4 v_color;
in vec4 v_position;

uniform sampler2D u_diffuseMap;
uniform vec3 u_cameraPosition;
uniform float u_fogStartDistance;
uniform float u_fogEndDistance;
uniform vec4 u_fogColorAndIntensity;

out vec4 fragment_color;


float FogDensity(vec3 pixelPosition)
{
	float distanceToCamera = distance(u_cameraPosition,pixelPosition);
	return clamp( (distanceToCamera - u_fogStartDistance) / (u_fogEndDistance - u_fogStartDistance) , 0.0 , 1.0);
}	


void main(void)
{
	vec4 diffuseColor = texture2D(u_diffuseMap,v_textureCoords);
	fragment_color = v_color * diffuseColor;

	vec3 colorDifference = (u_fogColorAndIntensity.xyz - fragment_color.xyz) * u_fogColorAndIntensity.a;
	float fog = FogDensity(v_position.xyz);
	fragment_color.rgb += (colorDifference * fog).rgb;
}