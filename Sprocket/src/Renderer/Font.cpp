#include "Font.h"

#include "Core/Global.h"

#include "ThirdParty/freetype/ft2build.h"
#include FT_FREETYPE_H

namespace Sprocket {

    Font::Font(const std::string& fontPath, const unsigned int fontSizeInPixels) : m_FontPath(fontPath) 
{
        // Init FreeType
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            Global::fileLogger.Error("Could not init FreeType.");
            std::exit(EXIT_FAILURE);
        }

        // Load font
        FT_Face face;
        if (FT_New_Face(ft, m_FontPath.c_str(), 0, &face)) {
            Global::fileLogger.Error(std::format("Failed to load font: {}", fontPath));
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
                Global::fileLogger.Warning(std::format("Failed to load glyph \"{}\" for {}", c, fontPath));
                continue;
            }

            FT_Bitmap& bmp = face->glyph->bitmap;

            // Vertical offset to preserve baseline
            int yOffset = maxTop - bmp.rows + (bmp.rows - face->glyph->bitmap_top);
            // Simplifies to:
            yOffset = maxTop - face->glyph->bitmap_top;

            for (int y = 0; y < bmp.rows; y++) {
                for (int x = 0; x < bmp.width; x++) {
                    int atlasX = xOffset + x;
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

}