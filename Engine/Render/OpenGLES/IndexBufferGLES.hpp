#pragma once

#include "Render/IndexBuffer.hpp"

class CIndexBufferGLES: public CIndexBuffer
{
public:
	CIndexBufferGLES();
	~CIndexBufferGLES();

	bool _UpdateSize();
	bool _Upload(void* _data, u32 _start, u32 _count);
};