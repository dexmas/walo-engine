#include "Render/OpenGLES/RenderGLES.hpp"
#include "Render/OpenGLES/IndexBufferGLES.hpp"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

CIndexBufferGLES::CIndexBufferGLES()
{
	glGenBuffers(1, &m_Handle);
}

CIndexBufferGLES::~CIndexBufferGLES()
{
	glDeleteBuffers(1, &m_Handle);
}

bool CIndexBufferGLES::_UpdateSize()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * m_IndexSize, 0, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
    
    TEST_GL_ERROR();

	return true;
}

bool CIndexBufferGLES::_Upload(void* _data, u32 _start, u32 _count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
    
    TEST_GL_ERROR();

	if(_start == 0 && _count == m_IndexCount)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * m_IndexSize, _data, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
	}
	else
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _start * m_IndexSize, _count * m_IndexSize, _data);
	}

	TEST_GL_ERROR();

	return true;
}
