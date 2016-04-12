#include "Render/Image.hpp"

#include <stb_image.h>

CImage::CImage():CAsset(EAT_IMAGE)
{
	m_Data = 0;
	m_Width = 0;
	m_Height = 0;
	m_Format = 0;
}

bool CImage::Load(CStream* _stream)
{
	u32 size = _stream->Size();
	u8* buffer = new u8[size];
	s32 width, height, bpp;

	if(size != _stream->Read(buffer, size))
	{
		delete buffer;
		return false;
	}

	m_Data = stbi_load_from_memory(buffer, size, &width, &height, &bpp, 0);

	delete buffer;

	if(!m_Data)
	{
		return false;
	}

	m_Width = width;
	m_Height = height;
	m_Format = bpp;

	return true;
}

void CImage::Unload()
{
	m_Width = 0;
	m_Height = 0;
	m_Format = 0;

	if(m_Data)
	{
		delete m_Data;
	}
}