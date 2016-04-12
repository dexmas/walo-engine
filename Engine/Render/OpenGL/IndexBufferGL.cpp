#include "Render/OpenGL/IndexBufferGL.hpp"

#include <glew.h>

CIndexBufferGL::CIndexBufferGL()
{
	glGenBuffers(1, &m_Handle);
}

CIndexBufferGL::~CIndexBufferGL()
{
	glDeleteBuffers(1, &m_Handle);
}

bool CIndexBufferGL::_UpdateSize()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * m_IndexSize, 0, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);

	return true;
}

bool CIndexBufferGL::_Upload(void* _data, u32 _start, u32 _count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);

	if(_start == 0 && _count == m_IndexCount)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * m_IndexSize, _data, m_Dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW);
	}
	else
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _start * m_IndexSize, _count * m_IndexSize, _data);
	}

	GLenum err = glGetError();

	return true;
}
