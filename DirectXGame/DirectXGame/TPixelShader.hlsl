Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);



struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float time;
	float alpha;
};

float4 tpsmain(PS_INPUT input) : SV_TARGET
{
	float4 pixelColor = Texture.Sample(TextureSampler,input.texcoord);
	clip(pixelColor.a < 0.1f ? -1 : 1);

	// do alpha testing

	return  pixelColor;

	




}