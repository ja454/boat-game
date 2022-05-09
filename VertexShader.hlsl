struct VS_INPUT
{
	//float3 inPos : POSITION;
	float2 inTexCoord : TEXCOORD;
	float2 inPos : POSITION;
	//float3 inColor : COLOR;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float2 outTexCoord : TEXCOORD;
	//float3 outColor : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.outPosition = float4(input.inPos, 0.0f, 1.0f);
	output.outTexCoord = input.inTexCoord;
	//output.outColor = input.inColor;
	return output;
}