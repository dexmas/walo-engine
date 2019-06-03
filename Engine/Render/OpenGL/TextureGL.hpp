#include "Render/Texture.hpp"

#if defined(WALO_PLATFORM_WIN32)
#include <glew.h>
#elif defined(WALO_PLATFORM_OSX)
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#endif

const GLenum g_GLTextureWrap[] =
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
};


class CTextureGL: public CTexture
{
public:
	CTextureGL();
	~CTextureGL();

protected:
	bool _UpdateSize();
	bool _Upload(void* _data, u32 _x, u32 _y, u32 _w, u32 _h);
};
