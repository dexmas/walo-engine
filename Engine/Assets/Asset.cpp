#include "Assets/Asset.hpp"
#include "Assets/AssetsDB.hpp"

CAsset::CAsset(EAssetType _type)
{
	m_Type = _type;
	m_Loaded = false;
	m_Cached = false;
}

CAsset::~CAsset()
{
	if (m_Cached)
	{
		int i = 0;
	}

	_UnloadInternal();

	if(m_Cached)
	{
		CAssetsDB::Instance()->_Remove(this);
	}
}

bool CAsset::_LoadInternal(CStream* _stream)
{
	if(m_Cached || m_Loaded)
	{
		return false;
	}

	m_Loaded = Load(_stream);

	return m_Loaded;
}

void CAsset::_UnloadInternal()
{
	if(m_Cached || !m_Loaded)
	{
		return;
	}

	Unload();

	m_Loaded = false;
}