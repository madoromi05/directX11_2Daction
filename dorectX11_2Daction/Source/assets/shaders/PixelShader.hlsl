Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global : register(b0)
{
    matrix g_mWVP;
    float4 g_PolyColor;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float4 texColor = g_Texture.Sample(g_Sampler, input.Tex);
    return texColor * g_PolyColor;
}