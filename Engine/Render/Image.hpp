#pragma once

#include "Assets/Asset.hpp"

class CImage: public CAsset
{
public:
	CImage();

	u32 Width(){return m_Width;}
	u32 Height(){return m_Height;}
	u32 Format(){return m_Format;}
	void* Data(){return m_Data;}

	bool Load(CStream* _stream);
	void Unload();

private:
	void* m_Data;
	u32 m_Width;
	u32 m_Height;
	u32 m_Format;
};