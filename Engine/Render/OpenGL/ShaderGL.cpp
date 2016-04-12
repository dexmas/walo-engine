#include "Render/OpenGL/ShaderGL.hpp"
#include "Core/String.hpp"

#include <glew.h>

CShaderGL::CShaderGL(EShaderType _type):CShader(_type)
{
	m_Handle = -1;

	if(_type == EST_PIXEL)
	{
		m_Handle = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
	if(_type == EST_VERTEX)
	{
		m_Handle = glCreateShader(GL_VERTEX_SHADER);
	}
}

CShaderGL::~CShaderGL()
{
	glDeleteShader(m_Handle);
}

bool CShaderGL::_Upload(void* _data)
{
	glShaderSource(m_Handle, 1, (const char**)(&_data), 0);
	glCompileShader(m_Handle);

	s32 compilationStatus;
	glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &compilationStatus);

	return (compilationStatus != GL_FALSE);
}