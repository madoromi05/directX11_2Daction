cbuffer TextCB : register(b0)
{
    float4x4 g_ortho;
    float4   g_color;
};

Texture2D    g_atlas   : register(t0);
SamplerState g_sampler : register(s0);

struct PS_IN { float4 pos : SV_POSITION; float2 uv : TEXCOORD0; };

float4 main(PS_IN input) : SV_TARGET
{
    float alpha = g_atlas.Sample(g_sampler, input.uv).r;
    return float4(g_color.rgb, g_color.a * alpha);
}
