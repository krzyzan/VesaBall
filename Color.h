#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

//! Packed 0xAARRGGBB color, replacing DirectX 8's D3DCOLOR.
/*!
	Same bit layout as D3DCOLOR, so every existing 0xAARRGGBB literal
	(e.g. 0xFFFFFFFF, 0xFF7FAFFF) works completely unchanged.
*/
typedef uint32_t Color;

inline uint8_t ColorA(Color c)
{
	return (uint8_t)(c >> 24);
}
inline uint8_t ColorR(Color c)
{
	return (uint8_t)(c >> 16);
}
inline uint8_t ColorG(Color c)
{
	return (uint8_t)(c >> 8);
}
inline uint8_t ColorB(Color c)
{
	return (uint8_t)(c);
}

inline Color MakeColor(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	return ((Color)a << 24) | ((Color)r << 16) | ((Color)g << 8) | (Color)b;
}

//! Returns 'c' with its alpha channel replaced by 'alpha01' (0..1).
/*!
	Replaces the original's one D3DXCOLOR usage (SpriteEffect.cpp's alpha
	fade: build a D3DXCOLOR from a DWORD, change .a, convert back).
*/
inline Color ColorWithAlpha(Color c, float alpha01)
{
	uint8_t a = (uint8_t)(alpha01 * 255.0f);
	return (c & 0x00FFFFFFu) | ((Color)a << 24);
}

//! Returns the alpha channel of 'c' as a 0..1 float.
inline float ColorAlpha(Color c)
{
	return ColorA(c) / 255.0f;
}

#endif
