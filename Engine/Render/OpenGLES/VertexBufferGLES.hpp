#pragma once

#include "Config.hpp"

#include "Render/VertexBuffer.hpp"

#if defined(WALO_PLATFORM_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

const u32 g_GLVertexElementComponents[] =
{
    3, // EVE_POSITION
    3, // EVE_NORMAL
    4, // EVE_COLOR
    2, // EVE_TEXCOORD1
    2, // EVE_TEXCOORD2
	2, // EVE_TEXCOORD3
    2, // EVE_TEXCOORD4
	4  // EVE_TANGENT
};

const u32 g_GLVertexElementType[] =
{
    GL_FLOAT,			// EVE_POSITION
    GL_FLOAT,			// EVE_NORMAL
    GL_UNSIGNED_BYTE,	// EVE_COLOR
    GL_FLOAT,			// EVE_TEXCOORD1
    GL_FLOAT,			// EVE_TEXCOORD2
	GL_FLOAT,			// EVE_TEXCOORD3
    GL_FLOAT,			// EVE_TEXCOORD4
	GL_FLOAT			// EVE_TANGENT
};

const u32 g_GLElementNormalize[] =
{
    GL_FALSE, // EVE_POSITION
    GL_FALSE, // EVE_NORMAL
    GL_TRUE,  // EVE_COLOR
    GL_FALSE, // EVE_TEXCOORD1
    GL_FALSE, // EVE_TEXCOORD2
	GL_FALSE, // EVE_TEXCOORD3
    GL_FALSE, // EVE_TEXCOORD4
	GL_FALSE  // EVE_TANGENT
};

class CVertexBufferGLES: public CVertexBuffer
{
public:
	CVertexBufferGLES();
	~CVertexBufferGLES();

	bool _UpdateSize();
	bool _Upload(void* _data, u32 _start, u32 _count);
};