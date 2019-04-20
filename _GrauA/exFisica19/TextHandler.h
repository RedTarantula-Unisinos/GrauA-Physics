



#pragma once

class TextHandler
{
public:
	TextHandler();
	~TextHandler();

	void InitTextHandler();
	void TH_SetFontSize(int size) { FT_Set_Pixel_Sizes(ftf, 0,size); };
	void TH_LoadFont();


private:

	FT_Library ftl;
	FT_Face ftf;
};

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	float Sizex;
	float Sizey;       // Size of glyph
	float Bearingx;
	float Bearingy;
	GLuint     Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;