#include "Render/OpenGLES/RenderGLES.hpp"
#include "Render/OpenGLES/ShaderGLES.hpp"
#include "Core/String.hpp"

#if defined(WALO_PLATFORM_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

CShaderGLES::CShaderGLES(EShaderType _type):CShader(_type)
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

CShaderGLES::~CShaderGLES()
{
	glDeleteShader(m_Handle);
}

bool CShaderGLES::_Upload(const char* _data)
{
    CString shader = "#version 100\n";
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
           
           delete log;
       }
    }

	return (compilationStatus != GL_FALSE);
}
