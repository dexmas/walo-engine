#include "Render/Render.hpp"
#include "Game.hpp"

#if defined(WALO_PLATFORM_WIN32)
#include "Render/OpenGL/RenderGL.hpp"
//#include "Render/DirectX11/RenderDX11.hpp"
#elif defined(WALO_PLATFORM_ANDROID)
#include "Render/OpenGLES/RenderGLES.hpp"
#elif defined(WALO_PLATFORM_LINUX)
#include "Render/OpenGL/RenderGL.hpp"
#elif defined(WALO_PLATFORM_IOS)
#include "Render/OpenGLES/RenderGLES.hpp"
#elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
#include "Render/DirectX11/RenderDX11.hpp"
#endif

CString kViewProjUniformName = "uViewProj";
CString kModelUniformName = "uModel";

CRender::CRender()
{
	m_Type = ERT_NULL;

	m_Device = CGame::Instance()->GetDevice();
	m_ClearColor = CColor::BLACK;
	m_Material = 0;
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_ClipEnabled = false;
}

CRender* CRender::CreateRender()
{
#if defined(WALO_PLATFORM_WIN32)
	return new CRenderGL();
	//return new CRenderDX11();
#elif defined(WALO_PLATFORM_ANDROID)
	return new CRenderGLES();
#elif defined(WALO_PLATFORM_IOS)
	return new CRenderGLES();
#elif defined(WALO_PLATFORM_WINPH) || defined(WALO_PLATFORM_WINRT)
	return new CRenderDX11();
#elif defined(WALO_PLATFORM_LINUX)
	return new CRenderGL();
#else
	return 0;
#endif
}

void CRender::ResetState()
{
	// TODO: Get native device resolution
	m_Viewport = CRect(0, 0, m_Device->GetScreenSize().X, m_Device->GetScreenSize().Y);
	_SetViewport(m_Viewport);

	m_ColorWrite = true;
	m_DepthWrite = true;

	_SetColorWrite(m_ColorWrite);
    _SetDepthWrite(m_DepthWrite);

	m_BlendMode = EBM_REPLACE;
	m_CullMode = ECM_NONE;
	m_FillMode = EFM_SOLID;
	m_DepthTestMode = EDTM_LESSEQUAL;

	_SetBlendMode(m_BlendMode);
	_SetCullMode(m_CullMode);
	_SetFillMode(m_FillMode);
    _SetDepthTest(m_DepthTestMode);
	_SetScissor(false, m_ClipRect);

	for(u32 i=0; i<ETC_COUNT; i++)
	{
		m_Textures[i] = 0;
		m_TexturesEnabled[i] = false;
		m_TexturesWrapU[i] = ETW_CLAMP;
		m_TexturesWrapV[i] = ETW_CLAMP;
		m_TexturesFilter[i] = ETF_LINEAR;
	}

	for(u32 i=0; i<ETT_COUNT; i++)
	{
		m_Transformation[i] = CMatrix4::IDENTY;
	}
}

CTexture* CRender::CreateTexture()
{
	return new CTexture();
}

CShader* CRender::CreateShader(EShaderType _type)
{
	if(_type >= EST_COUNT)
		return 0;

	return new CShader(_type);
}

CVertexBuffer* CRender::CreateVertexBuffer()
{
	return new CVertexBuffer();
}

CIndexBuffer* CRender::CreateIndexBuffer()
{
	return new CIndexBuffer();
}

CVertexBuffer* CRender::GetVertexBuffer()
{
	return m_VertexBuffer;
}

CIndexBuffer* CRender::GetIndexBuffer()
{
	return m_IndexBuffer;
}

const CMatrix4& CRender::GetTransform(ETransformType _type) const
{
	return m_Transformation[_type];
}

void CRender::SetClearColor(CColor& _clearColor) 
{
	m_ClearColor = _clearColor;
}

void CRender::SetMaterial(CMaterial* _material)
{
	if(!_material)
		return;

	if(_material != m_Material)
	{
		m_Material = _material;

		_SetColorWrite(_material->GetColorWrite());
		_SetDepthWrite(_material->GetDepthWrite());
		_SetBlendMode(_material->GetBlendMode());
		_SetCullMode(_material->GetCullMode());
		_SetFillMode(_material->GetFillMode());
		_SetDepthTest(_material->GetDepthTestMode());

		CShader* vsh = _material->GetShader(EST_VERTEX);
		CShader* psh = _material->GetShader(EST_PIXEL);

		if(m_VertexShader != vsh)
		{
			_SetShader(EST_VERTEX, vsh);
		}

		if(m_PixelShader != psh)
		{
			_SetShader(EST_PIXEL, psh);
		}
	}

	for(u32 i=0; i<ETC_COUNT; i++)
	{
		ETextureChanel slot = (ETextureChanel)i;

		CTexture* tex = _material->GetTexture(slot);

		if(m_Textures[i] != tex)
		{
			m_Textures[i] = tex;

			_SetTexture(slot, tex);

			if(tex)
			{
				// TODO: Check for already seted params
				_SetTextureUWrap(slot, _material->GetTextureUWrap(slot));
				_SetTextureVWrap(slot, _material->GetTextureVWrap(slot));
				_SetTextureFilter(slot, _material->GetTextureFilter(slot));
			}
		}
	}

	for(u32 i=0;i<EST_COUNT;i++)
	{
		CMap<CString, CShaderParameter>::CIterator it = m_Material->GetShaderParameters((EShaderType)i).Iterator();

		while(!it.AtEnd())
		{
			CShaderParameter& param = (*it).Value;
			CString& name = (*it).Key;
			void* value =  param.GetValue();

			if(value)
			{
				_SetShaderParameter(name, param.GetType(), value);
			}

			it++;
		}
	}
}

CMaterial* CRender::GetMaterial()
{
	return m_Material;
}

void CRender::SetVertexBuffer(CVertexBuffer* _buffer)
{
	if(m_VertexBuffer == _buffer)
		return;

	m_VertexBuffer = _buffer;

	_SetVertexBuffer(_buffer);
}

void CRender::SetIndexBuffer(CIndexBuffer* _buffer)
{
	if(m_IndexBuffer == _buffer)
		return;

	m_IndexBuffer = _buffer;

	_SetIndexBuffer(_buffer);
}

void CRender::SetTransform(ETransformType _type, const CMatrix4& _matrix)
{
	m_Transformation[_type] = _matrix;
}

void CRender::SetScissor(bool _clip, const CRect& _rect)
{
	m_ClipEnabled = _clip;
	m_ClipRect = _rect;

	_SetScissor(m_ClipEnabled, m_ClipRect);
}

bool CRender::Init()
{
	ResetState();

	return true;
}

void CRender::BeginFrame()
{
	m_ColorWrite = true;
	m_DepthWrite = true;

	_SetColorWrite(m_ColorWrite);
    _SetDepthWrite(m_DepthWrite);
}

void CRender::EndFrame()
{
	m_Device->Flush();
}

void CRender::Clear(bool _color, bool _depth, f32 _depthValue)
{
}

bool CRender::Ready()
{
	return m_Device->GetActive();
}

void CRender::Render(EPrimitiveType _ptype, u32 _vstart, u32 _vcount)
{
	_SetShaderParameter(kViewProjUniformName, ESPT_MATRIX4, m_Transformation[ETT_VIEWPROJ].M);
	_SetShaderParameter(kModelUniformName, ESPT_MATRIX4, m_Transformation[ETT_MODEL].M);
}

void CRender::RenderIndexed(EPrimitiveType _ptype, u32 _istart, u32 _icount)
{
	_SetShaderParameter(kViewProjUniformName, ESPT_MATRIX4, m_Transformation[ETT_VIEWPROJ].M);
	_SetShaderParameter(kModelUniformName, ESPT_MATRIX4, m_Transformation[ETT_MODEL].M);
}
