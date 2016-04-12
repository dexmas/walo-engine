#include "Render/OpenGL/RenderGL.hpp"

#include "Device/Device.hpp"
#include "Game.hpp"

#include "Render/OpenGL/TextureGL.hpp"
#include "Render/OpenGL/VertexBufferGL.hpp"
#include "Render/OpenGL/IndexBufferGL.hpp"

#include <glew.h>

static const u32 g_GLSrcBlend[] =
{
    GL_ONE,					// EBM_REPLACE
    GL_ONE,					// EBM_ADD
    GL_DST_COLOR,			// EBM_MULTIPLY
    GL_SRC_ALPHA,			// EBM_ALPHA
    GL_SRC_ALPHA,			// EBM_ADDALPHA
    GL_ONE,					// EBM_PREMULALPHA
    GL_ONE_MINUS_DST_ALPHA, // EBM_INVDESTALPHA
    GL_ONE,					// EBM_SUBTRACT
    GL_SRC_ALPHA			// EBM_SUBTRACTALPHA
};

static const u32 g_GLDestBlend[] =
{
    GL_ZERO,				// EBM_REPLACE
    GL_ONE,					// EBM_ADD
    GL_ZERO,				// EBM_MULTIPLY
    GL_ONE_MINUS_SRC_ALPHA, // EBM_ALPHA
    GL_ONE,					// EBM_ADDALPHA
    GL_ONE_MINUS_SRC_ALPHA, // EBM_PREMULALPHA
    GL_DST_ALPHA,			// EBM_INVDESTALPHA
    GL_ONE,					// EBM_SUBTRACT
    GL_ONE					// EBM_SUBTRACTALPHA
};

/*static const u32 g_GLBlendOp[] =
{
    GL_FUNC_ADD,				// EBM_REPLACE
    GL_FUNC_ADD,				// EBM_ADD
    GL_FUNC_ADD,				// EBM_MULTIPLY
    GL_FUNC_ADD,				// EBM_ALPHA
    GL_FUNC_ADD,				// EBM_ADDALPHA
    GL_FUNC_ADD,				// EBM_PREMULALPHA
    GL_FUNC_ADD,				// EBM_INVDESTALPHA
    GL_FUNC_REVERSE_SUBTRACT,	// EBM_SUBTRACT
    GL_FUNC_REVERSE_SUBTRACT	// EBM_SUBTRACTALPHA
};*/

static const u32 g_GLFillMode[] =
{
    GL_FILL, // EFM_SOLID
    GL_LINE, // EFM_WIREFRAME
    GL_POINT // EFM_POINT
};

static const u32 g_GLCmpFunc[] =
{
	GL_NONE,	 // EDTM_DISABLED
    GL_ALWAYS,   // EDTM_ALWAYS
    GL_EQUAL,	 // EDTM_EQUAL
    GL_NOTEQUAL, // EDTM_NOTEQUAL
    GL_LESS,	 // EDTM_LESS
    GL_LEQUAL,	 // EDTM_LESSEQUAL
    GL_GREATER,  // EDTM_GREATER
    GL_GEQUAL	 // EDTM_GREATEREQUAL
};

static const u32 g_GLVertexAttrIndex[] =
{
    0, // EVE_POSITION
	1, // EVE_NORMAL
	2, // EVE_COLOR
	3, // EVE_TEXCOORD1
	4, // EVE_TEXCOORD2
	5, // EVE_TEXCOORD3
	6, // EVE_TEXCOORD4
	7  // EVE_TANGENT
};

CRenderGL::CRenderGL()
{
	m_Type = ERT_GL;
	m_ActiveTexture = -1;
	m_CurrentProgram = 0;
	m_EnablesAttributes = 0;
}

void CRenderGL::_SetColorWrite(bool _enabled)
{
    if(_enabled)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    else
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
}

void CRenderGL::_SetDepthWrite(bool _enabled)
{
    glDepthMask(_enabled ? GL_TRUE : GL_FALSE);
}

void CRenderGL::_SetBlendMode(EBlendMode _mode)
{
    if (_mode == EBM_REPLACE)
        glDisable(GL_BLEND);
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc(g_GLSrcBlend[_mode], g_GLDestBlend[_mode]);
    }
}

void CRenderGL::_SetCullMode(ECullMode _mode)
{
    if(_mode == ECM_NONE)
        glDisable(GL_CULL_FACE);
    else
    {
        // Use Direct3D convention, ie. clockwise vertices define a front face
        glEnable(GL_CULL_FACE);
        glCullFace(_mode == ECM_CCW ? GL_FRONT : GL_BACK);
    }
}

void CRenderGL::_SetFillMode(EFillMode _mode)
{
    glPolygonMode(GL_FRONT_AND_BACK, g_GLFillMode[_mode]);
}

void CRenderGL::_SetDepthTest(EDepthTestMode _mode)
{
	if(_mode == EDTM_DISABLED)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(g_GLCmpFunc[_mode]);
	}
}

bool CRenderGL::Init()
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	CRender::Init();

	return true;
}

void CRenderGL::Clear(bool _color, bool _depth, f32 _depthValue)
{
	bool oldColorWrite = m_ColorWrite;
    bool oldDepthWrite = m_DepthWrite;
    
    if(_color && !oldColorWrite)
        _SetColorWrite(true);
    if(_depth && !oldDepthWrite)
        _SetDepthWrite(true);
    
    unsigned glFlags = 0;

    if(_color)
    {
        glFlags |= GL_COLOR_BUFFER_BIT;
        glClearColor(m_ClearColor.R, m_ClearColor.G, m_ClearColor.B, m_ClearColor.A);
    }
    if(_depth)
    {
        glFlags |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(_depthValue);
    }

	glDisable(GL_SCISSOR_TEST);
	
	glClear(glFlags);

	if(m_ClipEnabled)
	{
		glEnable(GL_SCISSOR_TEST);
	}
    
    _SetColorWrite(oldColorWrite);
    _SetDepthWrite(oldDepthWrite);
}

CTexture* CRenderGL::CreateTexture()
{
	return new CTextureGL();
}

CShader* CRenderGL::CreateShader(EShaderType _type)
{
	return new CShaderGL(_type);
}

CVertexBuffer* CRenderGL::CreateVertexBuffer()
{
	return new CVertexBufferGL();
}

CIndexBuffer* CRenderGL::CreateIndexBuffer()
{
	return new CIndexBufferGL();
}

void CRenderGL::_SetTexture(ETextureChanel _chanel, CTexture* _texture)
{
	if(m_ActiveTexture != _chanel)
	{
		glActiveTexture(GL_TEXTURE0 + (u32)_chanel);
		m_ActiveTexture = _chanel;
	}

	if(_texture)
	{
		if(!m_TexturesEnabled[_chanel])
		{
			m_TexturesEnabled[_chanel] = true;
			glEnable(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, _texture->GetHandle());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
}

void CRenderGL::_SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_GLTextureWrap[_wrap]);
}

void CRenderGL::_SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_GLTextureWrap[_wrap]);
}

void CRenderGL::_SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter)
{
	switch(_filter)
	{
	case ETF_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case ETF_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case ETF_ANISOTROPIC:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		f32 maxani;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxani);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxani);
		break;
	default:
		break;
	}
}

void CRenderGL::_SetShader(EShaderType _type, CShader* _shader)
{
	CShaderProgramGL* prog = 0;

	CShader* pixels = m_PixelShader;
	CShader* vertexs = m_VertexShader;

	if(_type == EST_VERTEX)
	{
		vertexs = _shader;
		m_VertexShader = vertexs;
	}
	else
	if(_type == EST_PIXEL)
	{
		pixels = _shader;
		m_PixelShader = pixels;
	}
	else
		return;

	CList<CShaderProgramGL*>::CIterator it = m_ShaderPrograms.Begin();
	while(it != m_ShaderPrograms.End())
	{
		if((*it)->GetPixelShader() == pixels && (*it)->GetVertexShader() == vertexs)
		{
			prog = *it;
		}
		++it;
	}

	if(!prog && pixels && vertexs)
	{
		prog = new CShaderProgramGL();

		prog->Link(vertexs, pixels);

		m_ShaderPrograms.PushBack(prog);
	}

	if(prog)
	{
		m_CurrentProgram = prog;
		glUseProgram(prog->GetHandle());
	}
}

void CRenderGL::_SetVertexBuffer(CVertexBuffer* _buffer)
{
	if(!_buffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _buffer->GetHandle());
    u32 vertexSize = _buffer->GetVertexSize();
	u32 elementMask = _buffer->GetElementMask();
        
    for(u32 j = 0; j < EVE_COUNT; ++j)
    {
        unsigned attrIndex = g_GLVertexAttrIndex[j];
        unsigned elementBit = 1 << j;
            
        if(elementMask & elementBit)
        {
            glEnableVertexAttribArray(attrIndex);
            glVertexAttribPointer(attrIndex, g_GLVertexElementComponents[j], g_GLVertexElementType[j], g_GLElementNormalize[j], vertexSize, (const GLvoid*)(_buffer->GetElementOffset((EVertexElement)j)));
        }
    }

	u32 disabledMask = ~elementMask;

	for(u32 j = 0; j < EVE_COUNT; ++j)
    {
        unsigned attrIndex = g_GLVertexAttrIndex[j];
        unsigned elementBit = 1 << j;
            
        if(disabledMask & elementBit)
        {
            glDisableVertexAttribArray(attrIndex);
        }
    }
}

void CRenderGL::_SetIndexBuffer(CIndexBuffer* _buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer->GetHandle());
}

void CRenderGL::_SetViewport(CRect& _viewport)
{
	glViewport(_viewport.X, _viewport.Y, _viewport.Width, _viewport.Height);
}

bool CRenderGL::_SetShaderParameter(CString& _name, EShaderParamType _type, void* _param)
{
	if(m_CurrentProgram)
	{
		return m_CurrentProgram->SetShaderParameter(_name, _type, _param);
	}

	return false;
}

void CRenderGL::_SetScissor(bool _clip, const CRect& _rect)
{
	if(_clip)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(_rect.X, m_Viewport.Height - _rect.Y - _rect.Height, _rect.Width, _rect.Height);
	}
	else
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

void CRenderGL::Render(EPrimitiveType _ptype, u32 _vstart, u32 _vcount)
{
	CRender::Render(_ptype, _vstart, _vcount);

	switch(_ptype)
    {
    case EPT_TRIANGLES:
        glDrawArrays(GL_TRIANGLES, _vstart, _vcount);
        break;
    case EPT_LINES:
        glDrawArrays(GL_LINES, _vstart, _vcount);
        break;
    }
}

void CRenderGL::RenderIndexed(EPrimitiveType _ptype, u32 _istart, u32 _icount)
{
	CRender::Render(_ptype, _istart, _icount);

	u32 indexSize = m_IndexBuffer->GetIndexSize();
    
    switch(_ptype)
    {
    case EPT_TRIANGLES:
        if(indexSize == sizeof(u16))
            glDrawElements(GL_TRIANGLES, _icount, GL_UNSIGNED_SHORT, (const GLvoid*)(_istart * indexSize));
        else
            glDrawElements(GL_TRIANGLES, _icount, GL_UNSIGNED_INT, (const GLvoid*)(_istart * indexSize));
        break;
        
    case EPT_LINES:
        if (indexSize == sizeof(u16))
            glDrawElements(GL_LINES, _icount, GL_UNSIGNED_SHORT, (const GLvoid*)(_istart * indexSize));
        else
            glDrawElements(GL_LINES, _icount, GL_UNSIGNED_INT, (const GLvoid*)(_istart * indexSize));
        break;
    }
}