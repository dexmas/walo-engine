#pragma once

#include "Assets/Asset.hpp"

enum EShaderType
{
	EST_VERTEX = 0,
	EST_PIXEL,

	EST_COUNT
};

class CShader: public CAsset
{
public:
	CShader(EShaderType _type):CAsset(EAT_SHADER){m_Type = _type;}

	u32 GetHandle() {return m_Handle;}

	bool Load(CStream* _stream);
	void Unload();

protected:
	virtual bool _Upload(void* _data){return true;}

	u32 m_Handle;
	EShaderType m_Type;
};