#pragma once

#include "Core/String.hpp"
#include "FileSystem/Stream.hpp"

enum EAssetType
{
	EAT_IMAGE = 0,
	EAT_TEXTURE,
	EAT_MATERIAL,
	EAT_MESH,
	EAT_SHADER,
	EAT_FONT2D,
	EAT_SOUND,

	EAT_COUNT
};

class CAsset
{
	friend class CAssetsDB;
public:

	CAsset(EAssetType _type);
	virtual ~CAsset();

	EAssetType Type() {return m_Type;}

	virtual bool Load(CStream* _stream){return true;}
	virtual void Unload(){}

private:
	void _SetCached(bool _cached){m_Cached = _cached;}
	bool _LoadInternal(CStream* _stream);
	void _UnloadInternal();
	
	bool m_Cached;
	bool m_Loaded;

	EAssetType m_Type;
};