#include "Render/Texture.hpp"
#include "Render/Image.hpp"
#include "Assets/AssetsDB.hpp"

CTexture::CTexture():CAsset(EAT_TEXTURE)
{
	m_TextureW = 0;
	m_TextureH = 0;
	m_ImageW = 0;
	m_ImageH = 0;
}

bool CTexture::Load(CStream* _stream)
{
	CImage image;
	
	if(!image.Load(_stream))
	{
		return false;
	}

	m_ImageH = image.Height();
	m_ImageW = image.Width();
	m_Components = image.Format();

	m_TextureW = CMath::NextPower2(m_ImageW);
	m_TextureH = CMath::NextPower2(m_ImageH);

	if(!_UpdateSize())
	{
		return false;
	}

	if(!_Upload(image.Data(), 0, 0, m_ImageW, m_ImageH))
	{
		return false;
	}

	return true;
}

void CTexture::Unload()
{
}