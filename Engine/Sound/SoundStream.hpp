#pragma once

#include "Assets/Asset.hpp"

enum ESoundFormat
{
	ESF_WAV,
	ESF_OGG,

	ESF_COUNT
};

class CSoundStream: public CAsset
{
public:
	CSoundStream();

	static CSoundStream* CreateStream(ESoundFormat _fmt);

	virtual bool Load(CStream* _stream);
	virtual void Unload();

	virtual u32  Read(char* _data, u32 _size = -1) = 0;
	virtual void Seek(u32 _pos) = 0;
	virtual u32  Pos() = 0;
	virtual u32  Size() {return m_DataSize;}

	u32 GetChannelsCount(){return m_ChannelsCount;}
	u32 GetSamplesPerSec(){return m_SamplesPerSec;}
	u32 GetBitsPerSample(){return m_BitsPerSample;}

protected:
	CStream* m_DataStream;
	u32		 m_DataSize;

	u32 m_ChannelsCount;
	u32 m_SamplesPerSec;
	u32 m_BitsPerSample;
};