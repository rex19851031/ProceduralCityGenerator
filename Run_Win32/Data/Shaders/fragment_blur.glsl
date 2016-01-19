#version 130
in vec2 v_textureCoords;
in vec4 v_color;
in vec4 v_position;

uniform sampler2D u_colorMap;
uniform sampler2D u_depthMap;

uniform float u_attenuation;
uniform vec2 u_sampleOffset;

out vec4 fragment_color;

float weights[21];

void main()
{ 
	weights[0] = 0.009167927656011385;
	weights[1] = 0.014053461291849008;
	weights[2] = 0.020595286319257878;
	weights[3] = 0.028855245532226279;
	weights[4] = 0.038650411513543079;
	weights[5] = 0.049494378859311142;
	weights[6] = 0.060594058578763078;
	weights[7] = 0.070921288047096992;
	weights[8] = 0.079358891804948081;
	weights[9] = 0.084895951965930902;
	weights[10] = 0.086826196862124602;
	weights[11] = 0.084895951965930902;
	weights[12] = 0.079358891804948081;
	weights[13] = 0.070921288047096992;
	weights[14] = 0.060594058578763092;
	weights[15] = 0.049494378859311121;
	weights[16] = 0.038650411513543079;
	weights[17] = 0.028855245532226279;
	weights[18] = 0.020595286319257885;
	weights[19] = 0.014053461291849008;
	weights[20] = 0.009167927656011385;


	vec3 sum = vec3( 0.0, 0.0, 0.0 );
	vec2 baseOffset = v_textureCoords + (-10.0 * u_sampleOffset);

	for( int s = 0; s < 21; ++s ) 
	{
		sum += texture2D( u_colorMap, baseOffset).rgb * weights[s];
		baseOffset += u_sampleOffset;
	} 

	fragment_color = vec4( sum.rgb * u_attenuation , 1.0 );
	//fragment_color = vec4(1,0,0,1);
}