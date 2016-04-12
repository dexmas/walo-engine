#pragma once

#include "Render/IndexBuffer.hpp"

class CIndexBufferGL: public CIndexBuffer
{
public:
	CIndexBufferGL();
	~CIndexBufferGL();

	bool _UpdateSize();
	bool _Upload(void* _data, u32 _start, u32 _count);
};