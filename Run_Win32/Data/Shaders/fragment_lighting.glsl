#version 130
in vec2 v_textureCoords;
in vec4 v_color;
in vec3 v_position;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_biTangent;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;

uniform vec3 u_cameraPosition;
uniform vec3 u_lightPosition;
uniform float u_fogStartDistance;
uniform float u_fogEndDistance;
uniform vec4 u_fogColorAndIntensity;

out vec4 fragment_color;


float FogDensity(vec3 pixelPosition)
{
	float distanceToCamera = distance(u_cameraPosition,pixelPosition);
	return clamp( (distanceToCamera - u_fogStartDistance) / (u_fogEndDistance - u_fogStartDistance) , 0.0 , 1.0);
}	


float LightDensity(vec4 pixelPosition , vec4 lightPosition , vec4 normalValue)
{	
	vec4 direction = lightPosition - pixelPosition;
	direction = normalize( direction );
	return clamp( dot(normalValue.xyz,direction.xyz) , 0 , 1 );
}


vec3 LightReflect(vec4 pixelPosition , vec4 lightPosition , vec3 lightColor , vec4 specularColor , vec4 normalValue)
{				
	vec4 cameraToVertex = normalize(vec4(u_cameraPosition,0) - pixelPosition);
	vec3 perfectToVertex = reflect(cameraToVertex.xyz,normalValue.xyz);
	vec4 vertexToLight = normalize(pixelPosition - lightPosition);
	float lightSimilar = clamp( dot(perfectToVertex.xyz,vertexToLight.xyz) , 0 , 1 );
	float reflectivity  = specularColor.r;
	float glossiness = specularColor.g;
	float specularExponent = 1.0 + (glossiness * 64);
	float howGlare = reflectivity * pow(lightSimilar , specularExponent);
	return howGlare * lightColor;
}


void main(void)
{
	vec3 normal = normalize(v_normal);
	vec3 tangent = normalize(v_tangent);
	vec3 biTangent = normalize(v_biTangent);
	mat4 tangentToWorldMatrix = mat4( vec4(tangent,0), vec4(biTangent,0), vec4(normal,0) , vec4(0,0,0,1) );

	vec4 diffuseColor = v_color + texture2D(u_diffuseMap,v_textureCoords);
	vec4 normalColor = texture2D(u_normalMap,v_textureCoords);
	vec4 specularColor = texture2D(u_specularMap,v_textureCoords);
	vec4 normalValue = (normalColor * 2.0) - vec4( 1.0 , 1.0 , 1.0 , 1.0);
	
	normalValue = tangentToWorldMatrix * normalValue;
	
	vec4 glareColor = vec4( LightReflect( vec4(v_position.xyz,0) , vec4(u_lightPosition.xyz,0) , vec3(0.5,0.5,0.5) , specularColor , normalValue ) , 0.0 );
	float light = LightDensity( vec4(v_position.xyz,0) , vec4(u_lightPosition.xyz,0) , normalValue );
	fragment_color = vec4( (diffuseColor * light + glareColor * light).rgb + (diffuseColor * specularColor.b).rgb , 1.0);

	vec3 colorDifference = (u_fogColorAndIntensity.xyz - fragment_color.xyz) * u_fogColorAndIntensity.a;
	float fog = FogDensity(v_position.xyz);
	fragment_color.rgb += (colorDifference * fog).rgb;

	if(v_color == vec4( 1.0 , 0.0 , 1.0 , 1.0) )
		fragment_color = vec4( 1.0 , 0.0 , 1.0 , 1.0);
}