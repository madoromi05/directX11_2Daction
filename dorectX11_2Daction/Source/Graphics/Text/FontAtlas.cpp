#ifdef _DEBUG
#include "FontAtlas.h"
#include "Debug.h"
#include <vector>

namespace engine
{
    FontAtlas::~FontAtlas()
    {
        if (m_ftFace) FT_Done_Face(m_ftFace);
        if (m_ftLib)  FT_Done_FreeType(m_ftLib);
    }

    HRESULT FontAtlas::Init(ID3D11Device* pDevice, const char* fontPath, int pixelHeight)
    {
        if (FT_Init_FreeType(&m_ftLib))
        {
            DEBUG_LOG_ERROR("FreeType: FT_Init_FreeType failed");
            return E_FAIL;
        }
        if (FT_New_Face(m_ftLib, fontPath, 0, &m_ftFace))
        {
            DEBUG_LOG_ERROR("FreeType: FT_New_Face failed");
            return E_FAIL;
        }
        FT_Set_Pixel_Sizes(m_ftFace, 0, pixelHeight);
        m_lineHeight = pixelHeight;

        std::vector<unsigned char> atlas(kAtlasW * kAtlasH, 0);

        int penX = 1, penY = 1;

        for (int i = 0; i < kNumGlyphs; ++i)
        {
            char c = static_cast<char>(kFirstChar + i);
            if (FT_Load_Char(m_ftFace, static_cast<unsigned char>(c), FT_LOAD_RENDER))
                continue;

            FT_Bitmap& bmp = m_ftFace->glyph->bitmap;

            if (penX + (int)bmp.width + 1 > kAtlasW)
            {
                penX  = 1;
                penY += pixelHeight + 2;
            }

            for (unsigned int r = 0; r < bmp.rows; ++r)
            {
                int dy = penY + (int)r;
                if (dy >= kAtlasH) break;
                for (unsigned int col = 0; col < bmp.width; ++col)
                {
                    int dx = penX + (int)col;
                    if (dx >= kAtlasW) break;
                    atlas[dy * kAtlasW + dx] = bmp.buffer[r * bmp.pitch + col];
                }
            }

            GlyphInfo& g = m_glyphs[i];
            g.u0       = static_cast<float>(penX)             / kAtlasW;
            g.v0       = static_cast<float>(penY)             / kAtlasH;
            g.u1       = static_cast<float>(penX + bmp.width) / kAtlasW;
            g.v1       = static_cast<float>(penY + bmp.rows)  / kAtlasH;
            g.bmpW     = bmp.width;
            g.bmpH     = bmp.rows;
            g.bearingX = m_ftFace->glyph->bitmap_left;
            g.bearingY = m_ftFace->glyph->bitmap_top;
            g.advance  = static_cast<int>(m_ftFace->glyph->advance.x >> 6);

            penX += (int)bmp.width + 2;
        }

        D3D11_TEXTURE2D_DESC td{};
        td.Width            = kAtlasW;
        td.Height           = kAtlasH;
        td.MipLevels        = 1;
        td.ArraySize        = 1;
        td.Format           = DXGI_FORMAT_R8_UNORM;
        td.SampleDesc.Count = 1;
        td.Usage            = D3D11_USAGE_IMMUTABLE;
        td.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem     = atlas.data();
        sd.SysMemPitch = kAtlasW;

        HRESULT hr = pDevice->CreateTexture2D(&td, &sd, &m_pTexture);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("FontAtlas: CreateTexture2D failed"); return hr; }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format              = DXGI_FORMAT_R8_UNORM;
        srvd.ViewDimension       = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MipLevels = 1;

        hr = pDevice->CreateShaderResourceView(m_pTexture.Get(), &srvd, &m_pSRV);
        if (FAILED(hr)) { DEBUG_LOG_ERROR("FontAtlas: CreateShaderResourceView failed"); return hr; }

        return S_OK;
    }

    const GlyphInfo& FontAtlas::GetGlyph(char c) const
    {
        int idx = static_cast<unsigned char>(c) - kFirstChar;
        if (idx < 0 || idx >= kNumGlyphs) idx = 0;
        return m_glyphs[idx];
    }
}
#endif // _DEBUG
