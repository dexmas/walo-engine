
#include "Render/OpenGLES/RenderGLES.hpp"

#include "Device/Device.hpp"
#include "Game.hpp"

#include "Render/OpenGLES/TextureGLES.hpp"
#include "Render/OpenGLES/VertexBufferGLES.hpp"
#include "Render/OpenGLES/IndexBufferGLES.hpp"

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

//#TODO
/*static const u32 g_GLFillMode[] =
{
    GL_FILL, // EFM_SOLID
    GL_LINE, // EFM_WIREFRAME
    GL_POINT // EFM_POINT
};*/

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

CRenderGLES::CRenderGLES()
{
	m_Type = ERT_GL;
	m_ActiveTexture = -1;
	m_CurrentProgram = 0;
	m_EnablesAttributes = 0;
}

void CRenderGLES::_SetColorWrite(bool _enabled)
{
    if(_enabled)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    else
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetDepthWrite(bool _enabled)
{
    glDepthMask(_enabled ? GL_TRUE : GL_FALSE);
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetBlendMode(EBlendMode _mode)
{
    if (_mode == EBM_REPLACE)
        glDisable(GL_BLEND);
    else
    {
        glEnable(GL_BLEND);
        glBlendFunc(g_GLSrcBlend[_mode], g_GLDestBlend[_mode]);
    }
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetCullMode(ECullMode _mode)
{
    if(_mode == ECM_NONE)
        glDisable(GL_CULL_FACE);
    else
    {
        // Use Direct3D convention, ie. clockwise vertices define a front face
        glEnable(GL_CULL_FACE);
        glCullFace(_mode == ECM_CCW ? GL_FRONT : GL_BACK);
    }
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetFillMode(EFillMode _mode)
{
    //#TODO
    //glPolygonMode(GL_FRONT_AND_BACK, g_GLFillMode[_mode]);
}

void CRenderGLES::_SetDepthTest(EDepthTestMode _mode)
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
    
    TEST_GL_ERROR();
}

bool CRenderGLES::Init()
{
    if(!CGame::Instance()->GetDevice())
    {
        return false;
    }
    
#ifdef WALO_PLATFORM_IOS

#else
    m_RenderWidth = CGame::Instance()->GetDevice()->GetScreenSize().X;
    m_RenderHeight = CGame::Instance()->GetDevice()->GetScreenSize().Y;
#endif
    
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    TEST_GL_ERROR();

	CRender::Init();

	return true;
}

void CRenderGLES::Clear(bool _color, bool _depth, f32 _depthValue)
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
        glClearDepthf(_depthValue);
    }

	glClear(glFlags);
    
    TEST_GL_ERROR();
    
    _SetColorWrite(oldColorWrite);
    _SetDepthWrite(oldDepthWrite);
}

CTexture* CRenderGLES::CreateTexture()
{
	return new CTextureGLES();
}

CShader* CRenderGLES::CreateShader(EShaderType _type)
{
	return new CShaderGLES(_type);
}

CVertexBuffer* CRenderGLES::CreateVertexBuffer()
{
	return new CVertexBufferGLES();
}

CIndexBuffer* CRenderGLES::CreateIndexBuffer()
{
	return new CIndexBufferGLES();
}

void CRenderGLES::_SetTexture(ETextureChanel _chanel, CTexture* _texture)
{
	if(m_ActiveTexture != _chanel)
	{
		glActiveTexture(GL_TEXTURE0 + (u32)_chanel);
		m_ActiveTexture = _chanel;
        
        TEST_GL_ERROR();
	}

	if(_texture)
	{
		if(!m_TexturesEnabled[_chanel])
		{
			m_TexturesEnabled[_chanel] = true;
			//glEnable(GL_TEXTURE_2D);
            
            TEST_GL_ERROR();
		}
		glBindTexture(GL_TEXTURE_2D, _texture->GetHandle());
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		//glDisable(GL_TEXTURE_2D);
	}
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetTextureUWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, g_GLTextureWrap[_wrap]);
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetTextureVWrap(ETextureChanel _chanel, ETextureWrap _wrap)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, g_GLTextureWrap[_wrap]);
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetTextureFilter(ETextureChanel _chanel, ETextureFilter _filter)
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
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetShader(EShaderType _type, CShader* _shader)
{
	CShaderProgramGLES* prog = 0;

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

	CList<CShaderProgramGLES*>::CIterator it = m_ShaderPrograms.Begin();
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
		prog = new CShaderProgramGLES();

		prog->Link(vertexs, pixels);

		m_ShaderPrograms.PushBack(prog);
	}

	if(prog)
	{
		m_CurrentProgram = prog;
		glUseProgram(prog->GetHandle());
	}
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetVertexBuffer(CVertexBuffer* _buffer)
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
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetIndexBuffer(CIndexBuffer* _buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer->GetHandle());
    
    TEST_GL_ERROR();
}

void CRenderGLES::_SetViewport(CRect& _viewport)
{
	glViewport(_viewport.X, _viewport.Y, _viewport.Width, _viewport.Height);
    
    TEST_GL_ERROR();
}

bool CRenderGLES::_SetShaderParameter(CString& _name, EShaderParamType _type, void* _param)
{
	if(m_CurrentProgram)
	{
		return m_CurrentProgram->SetShaderParameter(_name, _type, _param);
	}

	return false;
}

void CRenderGLES::_SetScissor(bool _clip, const CRect& _rect)
{
	if(_clip)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(_rect.X, _rect.Y, _rect.Width, _rect.Height);
	}
	else
		glDisable(GL_SCISSOR_TEST);
}

void CRenderGLES::Render(EPrimitiveType _ptype, u32 _vstart, u32 _vcount)
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
    
    TEST_GL_ERROR();
}

void CRenderGLES::RenderIndexed(EPrimitiveType _ptype, u32 _istart, u32 _icount)
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
    
    TEST_GL_ERROR();
}

void CRenderGLES::BeginFrame()
{
    TEST_GL_ERROR();
    
    CRender::BeginFrame();
}

void CRenderGLES::EndFrame()
{
    TEST_GL_ERROR();
    
    glFlush();
    glBindRenderbuffer(GL_RENDERBUFFER, m_ViewRenderbuffer);
    
    TEST_GL_ERROR();
    
    CRender::EndFrame();
}

