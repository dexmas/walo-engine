#include "Render/OpenGL/TextureGL.hpp"

CTextureGL::CTextureGL()
{
	glGenTextures(1, &m_Handle);
}

CTextureGL::~CTextureGL()
{
	glDeleteTextures(1, &m_Handle);
}

bool CTextureGL::_UpdateSize()
{
	GLuint format = 0;
	GLuint intfmt = 0;

	switch(m_Components)
	{
	case 1:
		format = GL_ALPHA;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	if(!format)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_Handle);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_TextureW, m_TextureH, 0, format, GL_UNSIGNED_BYTE, 0);

	return true;
}

bool CTextureGL::_Upload(void* _data, u32 _x, u32 _y, u32 _w, u32 _h)
{
	GLuint format = 0;
	GLuint intfmt = 0;

	switch(m_Components)
	{
	case 1:
		format = GL_ALPHA;
		break;
	case 2:
		format = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	if(!format)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, m_Handle);

	if(_x == 0 && _y == 0 && _w == m_TextureW && _h == m_TextureH)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_TextureW, m_TextureH, 0, format, GL_UNSIGNED_BYTE, _data);
	}
	else
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, _x, _y, _w, _h, format, GL_UNSIGNED_BYTE, _data);
	}

	return true;
}