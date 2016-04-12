#pragma once

#include "Core/Color.hpp"
#include "Core/Map.hpp"
#include "Render/RenderTypes.hpp"

#include "Assets/Asset.hpp"
#include "Render/Texture.hpp"
#include "Render/Shader.hpp"
#include "Render/ShaderParameter.hpp"

namespace rapidjson
{
	template<typename CharType>
	struct UTF8;

	class CrtAllocator;

	template <typename BaseAllocator>
	class MemoryPoolAllocator;

	template <typename Encoding, typename Allocator>
	class GenericValue;

	typedef GenericValue<UTF8<char>, MemoryPoolAllocator<CrtAllocator> > Value;
}

class CMaterial: public CAsset
{
public:
	CMaterial();
	~CMaterial();

	void SetTexture(ETextureChanel _chanel, CTexture* _texture);
	void SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap);
	void SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap);
	void SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter);
	void SetShader(EShaderType _type, CShader* _shader);

	void SetBlendMode(EBlendMode _mode){m_BlendMode = _mode;}
	void SetCullMode(ECullMode _mode){m_CullMode = _mode;}
	void SetFillMode(EFillMode _mode){m_FillMode = _mode;}
	void SetDepthTestMode(EDepthTestMode _mode){m_DepthTestMode = _mode;}
	void SetColorWrite(bool _enabled){m_ColorWrite = _enabled;}
	void SetDepthWrite(bool _enabled){m_DepthWrite = true;}

	CShader* GetShader(EShaderType _type);
	CTexture* GetTexture(ETextureChanel _chanel);
	ETextureWrap GetTextureUWrap(ETextureChanel _chanel);
	ETextureWrap GetTextureVWrap(ETextureChanel _chanel);
	ETextureFilter GetTextureFilter(ETextureChanel _chanel);

	EBlendMode GetBlendMode(){return m_BlendMode;}
	ECullMode GetCullMode(){return m_CullMode;}
	EFillMode GetFillMode(){return m_FillMode;}
	EDepthTestMode GetDepthTestMode(){return m_DepthTestMode;}
	bool GetColorWrite(){return m_ColorWrite;}
	bool GetDepthWrite(){return m_DepthWrite;}

	void SetShaderParameter(EShaderType _type, const char* _name, float _value);
	void SetShaderParameter(EShaderType _type, const char* _name, const CVector2& _value);
	void SetShaderParameter(EShaderType _type, const char* _name, const CVector3& _value);
	void SetShaderParameter(EShaderType _type, const char* _name, const CMatrix4& _value);

	CMap<CString, CShaderParameter>& GetShaderParameters(EShaderType _type);

	bool Load(CStream* _stream);
	void Unload();

private:
	struct _STextureSlot
	{
		CTexture* Texture;

		ETextureWrap WrapU;
		ETextureWrap WrapV;

		ETextureFilter Filter;

		bool Dirty;
	};

	void _LoadTextureSlot(const rapidjson::Value& _value);
	void _LoadShader(EShaderType _type, const rapidjson::Value& _value);

	_STextureSlot  m_TextureSlots[ETC_COUNT];
	CShader*	   m_Shaders[EST_COUNT];
	EBlendMode	   m_BlendMode;
	ECullMode      m_CullMode;
	EFillMode      m_FillMode;
	EDepthTestMode m_DepthTestMode;
	bool		   m_ColorWrite;
	bool		   m_DepthWrite;

	CMap<CString, CShaderParameter> m_ShaderParameters[EST_COUNT];
};