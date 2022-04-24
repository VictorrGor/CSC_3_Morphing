struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

VS_OUTPUT main(VS_INPUT inp)
{
	VS_OUTPUT res = (VS_OUTPUT)0;
	res.Pos = float4(inp.Pos, 1.0);
	res.Color = inp.Color;
	return res;
}