#pragma once
// サンプラーの用途を示す。
// Wrap  : 地形など繰り返しテクスチャに使用する。
// Clamp : UIなど短時間表示のオブジェクトに使用する。
enum class SamplerType
{
    Wrap,
    Clamp,
};