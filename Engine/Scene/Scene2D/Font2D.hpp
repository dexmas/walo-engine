#pragma once

#include "Assets/Asset.hpp"
#include "Core/Map.hpp"
#include "Render/Render.hpp"

class CFont2D: public CAsset
{
public:
	struct SGlyph
	{
		s32 X;
		s32 Y;
		u32 W;
		u32 H;

		s32 Xo;
		s32 Yo;

		s32 Xa;

		u32 Chanel;
	};

	CFont2D();

	bool Load(CStream* _stream);
	void Unload();

	SGlyph* GetGlyph(u16 _char);
	CTexture* GetTexture() const {return m_Texture;}
	u32 GetSize() const {return m_FontSize;}

private:
	CTexture*			 m_Texture;
	CMap<u16, SGlyph>	 m_GlyphMap;
	u32					 m_FontSize;
};