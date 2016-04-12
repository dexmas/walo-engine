#include "Render/Texture.hpp"

#include <glew.h>

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