#include "Render/Material.hpp"
#include "Assets/AssetsDB.hpp"

#include <rapidjson/document.h>

CMaterial::CMaterial():CAsset(EAT_MATERIAL)
{
	for(u32 i=0; i<ETC_COUNT; i++)
	{
		m_TextureSlots[i].Texture = 0;
		m_TextureSlots[i].WrapU = ETW_REPEAT;
		m_TextureSlots[i].WrapV = ETW_REPEAT;
		m_TextureSlots[i].Filter = ETF_NEAREST;
	}

	for(u32 i=0; i<EST_COUNT; i++)
	{
		m_Shaders[i] = 0;
	}

	m_BlendMode = EBM_REPLACE;
	m_CullMode = ECM_NONE;
	m_FillMode = EFM_SOLID;
	m_DepthTestMode = EDTM_DISABLED;
	m_ColorWrite = true;
	m_DepthWrite = true;
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetTexture(ETextureChanel _chanel, CTexture* _texture)
{
	m_TextureSlots[_chanel].Texture = _texture;
	m_TextureSlots[_chanel].Dirty = true;
}

void CMaterial::SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
	m_TextureSlots[_chanel].WrapU = _wrap;
	m_TextureSlots[_chanel].Dirty = true;
}

void CMaterial::SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
	m_TextureSlots[_chanel].WrapV = _wrap;
	m_TextureSlots[_chanel].Dirty = true;
}

void CMaterial::SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter)
{
	m_TextureSlots[_chanel].Filter = _filter;
	m_TextureSlots[_chanel].Dirty = true;
}

void CMaterial::SetShader(EShaderType _type, CShader* _shader)
{
	if(_type == EST_VERTEX)
	{
		if(m_Shaders[EST_VERTEX])
		{
			delete m_Shaders[EST_VERTEX];
		}
		m_Shaders[EST_VERTEX] = _shader;
	}
	else
	if(_type == EST_PIXEL)
	{
		if(m_Shaders[EST_PIXEL])
		{
			delete m_Shaders[EST_PIXEL];
		}

		m_Shaders[EST_PIXEL] = _shader;
	}
}

CShader* CMaterial::GetShader(EShaderType _type)
{
	return m_Shaders[_type];
}

CTexture* CMaterial::GetTexture(ETextureChanel _chanel)
{
	return m_TextureSlots[_chanel].Texture;
}

ETextureWrap CMaterial::GetTextureUWrap(ETextureChanel _chanel)
{
	return m_TextureSlots[_chanel].WrapU;
}

ETextureWrap CMaterial::GetTextureVWrap(ETextureChanel _chanel)
{
	return m_TextureSlots[_chanel].WrapV;
}

ETextureFilter CMaterial::GetTextureFilter(ETextureChanel _chanel)
{
	return m_TextureSlots[_chanel].Filter;
}

bool CMaterial::Load(CStream* _stream)
{
	rapidjson::Document jsondoc;

	char* buffer = new char[_stream->Size() + 1];
	_stream->Read(buffer, _stream->Size());
	buffer[_stream->Size()] = 0;

	bool err = jsondoc.ParseInsitu<0>(buffer).HasParseError();

	if(err)
	{
		delete[] buffer;
		return false;
	}

	rapidjson::Value::ConstMemberIterator itr = jsondoc.MemberBegin();
	while(itr != jsondoc.MemberEnd())
	{
		CString name(itr->name.GetString());

		if(name == "Textures")
		{
			rapidjson::Value::ConstValueIterator jtr = itr->value.Begin();
				
			while(jtr != itr->value.End())
			{
				_LoadTextureSlot(*jtr);

				jtr++;
			}
		}
		else
		if(name == "VertexShader")
		{
			_LoadShader(EST_VERTEX, itr->value);
		}
		else
		if(name == "PixelShader")
		{
			_LoadShader(EST_PIXEL, itr->value);
		}
		else
		if(name == "ColorWrite")
		{
			if(itr->value.IsBool())
			{
				m_ColorWrite = itr->value.GetBool();
			}
		}
		else
		if(name == "DepthWrite")
		{
			if(itr->value.IsBool())
			{
				m_DepthWrite = itr->value.GetBool();
			}
		}
		else
		if(name == "BlendMode")
		{
			s32 val = itr->value.GetInt();

			if(val >= 0 && val < (s32)EBM_COUNT)
			{
				m_BlendMode = (EBlendMode)val;
			}
		}
		else
		if(name == "CullMode")
		{
			s32 val = itr->value.GetInt();

			if(val >= 0 && val < (s32)ECM_COUNT)
			{
				m_CullMode = (ECullMode)val;
			}
		}
		else
		if(name == "FillMode")
		{
			s32 val = itr->value.GetInt();

			if(val >= 0 && val < (s32)EFM_COUNT)
			{
				m_FillMode = (EFillMode)val;
			}
		}
		else
		if(name == "DepthTest")
		{
			s32 val = itr->value.GetInt();

			if(val >= 0 && val < (s32)EDTM_COUNT)
			{
				m_DepthTestMode = (EDepthTestMode)val;
			}
		}

		itr++;
	}

	return true;
}

void CMaterial::Unload()
{
	for(u32 i=0; i<ETC_COUNT; i++)
	{
		if(m_TextureSlots[i].Texture)
		{
			delete m_TextureSlots[i].Texture;
			m_TextureSlots[i].Texture = 0;
		}

		m_TextureSlots[i].WrapU = ETW_REPEAT;
		m_TextureSlots[i].WrapV = ETW_REPEAT;
		m_TextureSlots[i].Filter = ETF_NEAREST;
	}

	for(u32 i=0; i<EST_COUNT; i++)
	{
		if(m_Shaders[i])
		{
			delete m_Shaders[i];
			m_Shaders[i] = 0;
		}
	}

	m_BlendMode = EBM_REPLACE;
	m_CullMode = ECM_NONE;
	m_FillMode = EFM_SOLID;
	m_DepthTestMode = EDTM_DISABLED;
	m_ColorWrite = true;
	m_DepthWrite = true;
}

void CMaterial::SetShaderParameter(EShaderType _type, const char* _name, float _value)
{
	m_ShaderParameters[_type][_name] = CShaderParameter(_value);
}

void CMaterial::SetShaderParameter(EShaderType _type, const char* _name, const CVector2& _value)
{
	m_ShaderParameters[_type][_name] = CShaderParameter(_value);
}

void CMaterial::SetShaderParameter(EShaderType _type, const char* _name, const CVector3& _value)
{
	m_ShaderParameters[_type][_name] = CShaderParameter(_value);
}

void CMaterial::SetShaderParameter(EShaderType _type, const char* _name, const CMatrix4& _value)
{
	m_ShaderParameters[_type][_name] = CShaderParameter(_value);
}

CMap<CString, CShaderParameter>& CMaterial::GetShaderParameters(EShaderType _type)
{
	return m_ShaderParameters[_type];
}

void CMaterial::_LoadTextureSlot(const rapidjson::Value& _value)
{
	if(!_value.HasMember("Slot") || !_value.HasMember("File"))
		return;

	s32 slot = _value["Slot"].GetInt();
	CString filename = _value["File"].GetString();

	if(slot < 0 || slot >= ETC_COUNT || filename == "")
		return;

	if(m_TextureSlots[slot].Texture)
		delete m_TextureSlots[slot].Texture;

	m_TextureSlots[slot].Texture = CAssetsDB::Instance()->GetTexture(filename.CStr());

	if(_value.HasMember("WrapU"))
	{
		s32 val = _value["WrapU"].GetInt();

		if(val >= 0 && val < (s32)ETW_COUNT)
		{
			m_TextureSlots[slot].WrapU = (ETextureWrap)val;
		}
	}

	if(_value.HasMember("WrapV"))
	{
		s32 val = _value["WrapV"].GetInt();

		if(val >= 0 && val < (s32)ETW_COUNT)
		{
			m_TextureSlots[slot].WrapV = (ETextureWrap)val;
		}
	}

	if(_value.HasMember("Filter"))
	{
		s32 val = _value["Filter"].GetInt();

		if(val >= 0 && val < (s32)ETF_COUNT)
		{
			m_TextureSlots[slot].Filter = (ETextureFilter)val;
		}
	}
}

void CMaterial::_LoadShader(EShaderType _type, const rapidjson::Value& _value)
{
	if(!_value.HasMember("File"))
		return;

	CString filename = _value["File"].GetString();

	m_Shaders[_type] = CAssetsDB::Instance()->GetShader(filename.CStr());

	// TODO: Load shaders params
}