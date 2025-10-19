#include "Font.h"

#include "Core/Global.h"

#include "ThirdParty/freetype/ft2build.h"
#include FT_FREETYPE_H

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Font::Font(const std::string& fontPath, const unsigned int fontSizeInPixels) : m_FontPath(fontPath) 
{
        // Init FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            Global::FileLogger().Error("Could not init FreeType.");
            std::exit(EXIT_FAILURE);
        }

        // Load font
        FT_Face face;
        if (FT_New_Face(ft, m_FontPath.c_str(), 0, &face)) {
            Global::FileLogger().Error(std::format("Failed to load font: {}", fontPath));
            std::exit(EXIT_FAILURE);
        }

        FT_Set_Pixel_Sizes(face, 0, fontSizeInPixels);

        // First pass: compute atlas width and max top/bottom
        int maxTop = 0;
        int maxBottom = 0;
        int atlasWidth = 0;

        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

            atlasWidth += face->glyph->bitmap.width + 1; // +1 for spacing

            maxTop = std::max(maxTop, (int)face->glyph->bitmap_top);
            int bottom = face->glyph->bitmap.rows - face->glyph->bitmap_top;
            maxBottom = std::max(maxBottom, bottom);
        }

        m_Atlas.width = atlasWidth;
        m_Atlas.height = maxTop + maxBottom;
        m_Atlas.pixels.assign(m_Atlas.width * m_Atlas.height * 4, 0);

        // Second pass: render glyphs into atlas
        int xOffset = 0;
        for (unsigned char c = 0; c < 128; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                Global::FileLogger().Warning(std::format("Failed to load glyph \"{}\" for {}", c, fontPath));
                continue;
            }

            FT_Bitmap& bmp = face->glyph->bitmap;

            int yOffset = maxTop - face->glyph->bitmap_top;

            for (int y = 0; y < bmp.rows; y++) {
                for (int x = 0; x < bmp.width; x++) {
                    int atlasX = xOffset + x;
                    //int atlasY = yOffset + (bmp.rows - 1 - y);
                    int atlasY = yOffset + y;
                    int index = (atlasY * m_Atlas.width + atlasX) * 4;

                    unsigned char value = bmp.buffer[y * bmp.pitch + x];

                    // For now just printing white text always. This should be changed
                    m_Atlas.pixels[index + 0] = 255; // R
                    m_Atlas.pixels[index + 1] = 255; // G
                    m_Atlas.pixels[index + 2] = 255; // B
                    m_Atlas.pixels[index + 3] = value; // A
                }
            }

            // Store character info
            Character g;
            g.width = bmp.width;
            g.height = bmp.rows;
            g.bearing.x = face->glyph->bitmap_left;
            g.bearing.y = face->glyph->bitmap_top;
            g.advance = face->glyph->advance.x;

            // Normalized texture coordinates
            g.uv0.x = (float)xOffset / (float)m_Atlas.width;
            g.uv0.y = (float)yOffset / (float)m_Atlas.height;
            g.uv1.x = (float)(xOffset + bmp.width) / (float)m_Atlas.width;
            g.uv1.y = (float)(yOffset + bmp.rows) / (float)m_Atlas.height;

            m_Characters.insert({ c, g });
            xOffset += bmp.width + 1;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    std::vector<unsigned char> Font::GetTextureBufferForText(const std::string& text,
        unsigned int& width, unsigned int& height) {

        // Compute size
        width = 0;
        int maxAboveBaseline = 0;
        int maxBelowBaseline = 0;

        for (char c : text) {
            const Character& ch = m_Characters.at(c);

            width += (ch.advance >> 6); // FreeType advance is in 1/64th pixels
            maxAboveBaseline = std::max(maxAboveBaseline, (int)ch.bearing.y) + 1;
            maxBelowBaseline = std::max(maxBelowBaseline, (int)(ch.height - ch.bearing.y)) + 1;
        }

        height = maxAboveBaseline + maxBelowBaseline;

        // Allocate buffer that will return containing rgba data for the text
        std::vector<unsigned char> pixels(width * height * 4, 0);

        // Populate buffer
        int penX = 0;
        for (char c : text) {
            const Character& ch = m_Characters.at(c);

            int xpos = penX + ch.bearing.x;
            int ypos = maxAboveBaseline - ch.bearing.y;

            for (unsigned int row = 0; row < ch.height; ++row) {
                for (unsigned int col = 0; col < ch.width; ++col) {
                    int dstX = xpos + col;
                    int dstY = ypos + row;
                    if (dstX < 0 || dstX >= width || dstY < 0 || dstY >= height)
                        continue;

                    int srcX = (int)(ch.uv0.x * m_Atlas.width) + col;
                    int srcY = (int)(ch.uv0.y * m_Atlas.height) + row;

                    int srcIndex = (srcY * m_Atlas.width + srcX) * 4;
                    unsigned char alpha = m_Atlas.pixels[srcIndex + 3];

                    int dstIndex = (dstY * width + dstX) * 4;
                    pixels[dstIndex + 0] = 255;
                    pixels[dstIndex + 1] = 255;
                    pixels[dstIndex + 2] = 255;
                    pixels[dstIndex + 3] = std::max(pixels[dstIndex + 3], alpha);
                }
            }

            penX += (ch.advance >> 6);
        }

        return pixels;
    }

}