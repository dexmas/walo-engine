#include "Render/Shader.hpp"

bool CShader::Load(CStream* _stream)
{
	u32 len = _stream->Size();
	char* buffer = new char[len+1];
	_stream->Read(buffer, len);
	buffer[len] = 0;

	bool ret = _Upload(buffer);

	delete buffer;

	return ret;
}

void CShader::Unload()
{
}