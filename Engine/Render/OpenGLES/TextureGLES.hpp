#include "Render/Texture.hpp"

#if defined(WALO_PLATFORM_ANDROID)
#include <GLES2/gl2.h>
#else
#include <OpenGLES/ES2/gl.h>
#endif

const GLenum g_GLTextureWrap[] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
};


class CTextureGLES: public CTexture
{
public:
	CTextureGLES();
	~CTextureGLES();

protected:
	bool _UpdateSize();
	bool _Upload(void* _data, u32 _x, u32 _y, u32 _w, u32 _h);
};