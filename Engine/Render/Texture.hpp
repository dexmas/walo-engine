#pragma once

#include "Assets/Asset.hpp"

class CTexture: public CAsset
{
public:
	CTexture();

	u32 GetHandle() {return m_Handle;}

	bool Load(CStream* _stream);
	void Unload();

	u32 GetWidth(){return m_TextureW;}
	u32 GetHeight(){return m_TextureH;}

	u32 GetImageWidth(){return m_ImageW;}
	u32 GetImageHeigh(){return m_ImageH;}
	
protected:
	virtual bool _UpdateSize(){return true;}
	virtual bool _Upload(void* _data, u32 _x, u32 _y, u32 _w, u32 _h){return true;}

	u32 m_Components;
	u32 m_TextureW;
	u32 m_TextureH;
	u32 m_ImageW;
	u32 m_ImageH;

	u32 m_Handle;
};