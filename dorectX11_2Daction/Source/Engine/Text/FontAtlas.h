#pragma once
#ifdef _DEBUG

#include <d3d11.h>
#include <wrl/client.h>
#include <array>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine
{
    struct GlyphInfo
    {
        float u0, v0, u1, v1;
        int   bmpW, bmpH;
        int   bearingX, bearingY;
        int   advance;
    };

    class FontAtlas
    {
    public:
        static constexpr int kFirstChar = 32;
        static constexpr int kLastChar  = 126;
        static constexpr int kNumGlyphs = kLastChar - kFirstChar + 1;
        static constexpr int kAtlasW    = 512;
        static constexpr int kAtlasH    = 128;

        ~FontAtlas();
        HRESULT Init(ID3D11Device* pDevice, const char* fontPath, int pixelHeight);

        const GlyphInfo&          GetGlyph(char c)  const;
        ID3D11ShaderResourceView* GetSRV()           const { return m_pSRV.Get(); }
        int                       GetLineHeight()    const { return m_lineHeight; }

    private:
        std::array<GlyphInfo, kNumGlyphs>            m_glyphs{};
        Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_pTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
        int        m_lineHeight = 0;
        FT_Library m_ftLib  = nullptr;
        FT_Face    m_ftFace = nullptr;
    };
}

#endif
