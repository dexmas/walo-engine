#pragma once

#include "Config.hpp"
#include "Core/Log.hpp"

#if defined(WALO_PLATFORM_WIN32)
#include <glew.h>
#elif defined(WALO_PLATFORM_OSX)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#endif

#ifdef _DEBUG
static void TestGLError()
{
    GLenum errCode;
    if((errCode = glGetError() ) != GL_NO_ERROR)
    {
        LOG("! OpenGL Error (%s).\n", (char*)glGetString(errCode));
    }
}
#define TEST_GL_ERROR TestGLError
#else
#define TEST_GL_ERROR()
#endif

#include "Render/Render.hpp"
#include "Core/List.hpp"

#include "Render/OpenGL/ShaderProgramGL.hpp"

#ifdef WALO_PLATFORM_WIN32
#include <Windows.h>
#endif

class CRenderGL: public CRender
{
public:
	CRenderGL();

	bool Init();
	void Clear(bool _color, bool _depth, f32 _depthValue);

	CTexture*	   CreateTexture();
	CShader*	   CreateShader(EShaderType _type);
	CVertexBuffer* CreateVertexBuffer();
	CIndexBuffer*  CreateIndexBuffer();

	void Render(EPrimitiveType _ptype, u32 _vstart, u32 _vcount);
	void RenderIndexed(EPrimitiveType _ptype, u32 _istart, u32 _icount);

private:
	void _SetVertexBuffer(CVertexBuffer* _buffer);
	void _SetIndexBuffer(CIndexBuffer* _buffer);
	void _SetViewport(CRect& _viewport);
	void _SetTexture(ETextureChanel _chanel, CTexture* _texture);
	void _SetShader(EShaderType _type, CShader* _shader);
	void _SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap);
	void _SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap);
	void _SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter);
	void _SetColorWrite(bool _enabled);
	void _SetDepthWrite(bool _enabled);
	void _SetBlendMode(EBlendMode _mode);
	void _SetCullMode(ECullMode _mode);
	void _SetFillMode(EFillMode _mode);
	void _SetDepthTest(EDepthTestMode _mode);
	bool _SetShaderParameter(CString& _name, EShaderParamType _type, void* _param);
	void _SetScissor(bool _clip, const CRect& _rect);

	CList<CShaderProgramGL*> m_ShaderPrograms;
	CShaderProgramGL*		 m_CurrentProgram;
	u32 m_EnablesAttributes;
	u32 m_ActiveTexture;
};
