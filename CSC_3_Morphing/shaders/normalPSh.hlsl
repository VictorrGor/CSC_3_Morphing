struct PS_INTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(PS_INTPUT inp) : SV_TARGET
{

	return inp.Color;
}