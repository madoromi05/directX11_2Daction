// グローバル変数 (VertexShaderと同じ定義でOK)
cbuffer global : register(b0)
{
    matrix g_mWVP;
    float4 g_PolyColor;
};

// ピクセルシェーダー (関数名は PS)
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
    return g_PolyColor;
}