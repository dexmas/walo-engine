#pragma once

#include "Core/Types.hpp"
#include "Render/RenderTypes.hpp"

#include "Core/Matrix.hpp"
#include "Core/Vector.hpp"
#include "Core/Color.hpp"
#include "Core/Rect.hpp"
#include "Core/List.hpp"

#include "Device/Device.hpp"
#include "Render/Texture.hpp"
#include "Render/Shader.hpp"
#include "Render/VertexBuffer.hpp"
#include "Render/IndexBuffer.hpp"
#include "Render/Material.hpp"

static const int MAX_RENDERTARGETS = 4;
static const int MAX_VERTEX_STREAMS = 4;
static const int MAX_SKIN_MATRICES = 64;

class CGPUObject;
class CVertexBuffer;

class CRender
{
public:
	enum ERenderType
	{
		ERT_NULL = 0,
		ERT_GL,
		ERT_GLES,
		ERT_GLES2,
		ERT_DX11,

		ERT_COUNT
	};

	CRender();

	virtual ~CRender(){}

	static CRender* CreateRender();

	void SetClearColor(CColor& _clearColor);
	void SetMaterial(CMaterial* _material);
	void SetVertexBuffer(CVertexBuffer* _buffer);
	void SetIndexBuffer(CIndexBuffer* _buffer);
	void SetTransform(ETransformType _type, const CMatrix4& _matrix);
	void SetScissor(bool _clip, const CRect& _rect);

	CMaterial*	   GetMaterial();
	CVertexBuffer* GetVertexBuffer();
	CIndexBuffer*  GetIndexBuffer();
	const CMatrix4& GetTransform(ETransformType _type) const;

	void ResetState();

	virtual CTexture*	   CreateTexture();
	virtual CShader*	   CreateShader(EShaderType _type);
	virtual CVertexBuffer* CreateVertexBuffer();
	virtual CIndexBuffer*  CreateIndexBuffer();

	virtual bool Init();
	virtual void BeginFrame();
	virtual void EndFrame();
	virtual void Clear(bool _color, bool _depth, f32 _depthValue = 1.0f);
	virtual bool Ready();

	virtual void Render(EPrimitiveType _ptype, u32 _vstart, u32 _vcount);
	virtual void RenderIndexed(EPrimitiveType _ptype, u32 _istart, u32 _icount);

protected:
	virtual void _SetViewport(CRect& _viewport){}
	virtual void _SetVertexBuffer(CVertexBuffer* _buffer){}
	virtual void _SetIndexBuffer(CIndexBuffer* _buffer){}
	virtual void _SetTexture(ETextureChanel _chanel, CTexture* _texture){}
	virtual void _SetShader(EShaderType _type, CShader* _shader){}
	virtual void _SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap){}
	virtual void _SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap){}
	virtual void _SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter){}
	virtual void _SetColorWrite(bool _enabled){}
	virtual void _SetDepthWrite(bool _enabled){}
	virtual void _SetBlendMode(EBlendMode _mode){}
	virtual void _SetCullMode(ECullMode _mode){}
	virtual void _SetFillMode(EFillMode _mode){}
	virtual void _SetDepthTest(EDepthTestMode _mode){}
	virtual bool _SetShaderParameter(CString& _name, EShaderParamType _type, void* _param){return false;}
	virtual void _SetScissor(bool _clip, const CRect& _rect){}

	CDevice*	m_Device;
	ERenderType m_Type;
	CColor		m_ClearColor;

	bool		m_ColorWrite;
	bool		m_DepthWrite;

	CRect		   m_Viewport;
	EBlendMode	   m_BlendMode;
	ECullMode      m_CullMode;
	EFillMode      m_FillMode;
	EDepthTestMode m_DepthTestMode;
	bool		   m_ClipEnabled;
	CRect		   m_ClipRect;

	CMaterial*     m_Material;

	CVertexBuffer* m_VertexBuffer;
	CIndexBuffer*  m_IndexBuffer;
	CTexture*	   m_Textures[ETC_COUNT];
	bool		   m_TexturesEnabled[ETC_COUNT];
	ETextureWrap   m_TexturesWrapU[ETC_COUNT];
	ETextureWrap   m_TexturesWrapV[ETC_COUNT];
	ETextureFilter m_TexturesFilter[ETC_COUNT];
	CShader*	   m_VertexShader;
	CShader*	   m_PixelShader;
	CMatrix4  m_Transformation[ETT_COUNT];
};
