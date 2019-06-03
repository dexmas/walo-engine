#include "Render/OpenGL/VertexBufferGL.hpp"
#include "Render/OpenGL/RenderGL.hpp"

CVertexBufferGL::CVertexBufferGL()
{
	glGenBuffers(1, &m_Handle);
}

CVertexBufferGL::~CVertexBufferGL()
{
	glDeleteBuffers(1, &m_Handle);
}

bool CVertexBufferGL::_UpdateSize()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * m_VertexSize, 0, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
    
    TEST_GL_ERROR();

	return true;
}

bool CVertexBufferGL::_Upload(void* _data, u32 _start, u32 _count)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Handle);

    TEST_GL_ERROR();

	if(_start == 0 && _count == m_VertexCount)
	{
		glBufferData(GL_ARRAY_BUFFER, m_VertexCount * m_VertexSize, _data, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, _start * m_VertexSize, _count * m_VertexSize, _data);
	}

	TEST_GL_ERROR();

	return true;
}
