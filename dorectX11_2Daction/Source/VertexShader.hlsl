// グローバル変数
cbuffer global : register(b0)
{
    matrix g_mWVP;
    float4 g_PolyColor;
};

// 頂点シェーダー (関数名は VS)
float4 VS(float4 Pos : POSITION) : SV_POSITION
{
    Pos = mul(Pos, g_mWVP);
    return Pos;
}