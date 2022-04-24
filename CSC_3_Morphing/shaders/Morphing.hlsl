struct VS_INPUT
{
	float3 Pos : POSITION;
	float3 targetPos : TARGET_POS;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

cbuffer cb_frame : register(b0)
{
	float time;
};

cbuffer cb_model : register(b1)
{
	float4x4 mvp;
};


VS_OUTPUT main(VS_INPUT inp)
{
	VS_OUTPUT o = (VS_OUTPUT)0;
	//inp.Pos =  mul(float4(inp.Pos,1), mvp);
	//inp.targetPos = inp.targetPos * mvp;
	o.Pos = float4((1 - time) * inp.Pos + time * inp.targetPos, 1);
	//o.Pos = mul(mvp, float4(inp.Pos, 1));//float4(inp.Pos, 1.0);
	o.Color = inp.Color;
	return o;
}