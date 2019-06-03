#include "Render/OpenGL/ShaderGL.hpp"
#include "Render/OpenGL/RenderGL.hpp"
#include "Core/String.hpp"

#if defined(WALO_PLATFORM_WIN32)
#include <glew.h>
#elif defined(WALO_PLATFORM_OSX)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#endif

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

bool CShaderGL::_Upload(const char* _data)
{
    CString shader = "#version 110\n#define highp\n#define mediump\n#define lowp\n#define precision\n";
    shader += _data;
    
    const char* buffer = shader.CStr();
    
    glShaderSource(m_Handle, 1, &buffer, 0);
	glCompileShader(m_Handle);

	s32 compilationStatus;
	glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &compilationStatus);
    
    TEST_GL_ERROR();
    
    if(compilationStatus == GL_FALSE)
    {
       LOG("! Failed to compile shader: \n");
       
       GLint logLength;
       glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &logLength);
       
       if(logLength > 0)
       {
           unsigned char *log = new unsigned char[logLength];

           glGetShaderInfoLog(m_Handle, logLength, &logLength, (GLchar*)&log[0]);
           
           LOG("%s\n", log);
           
           delete[] log;
       }
    }
    
	return (compilationStatus != GL_FALSE);
}
