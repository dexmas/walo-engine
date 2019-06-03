#include "Core/String.hpp"
#include "Render/Shader.hpp"

bool CShader::Load(CStream* _stream)
{
    char* buffer = new char[_stream->Size()+1];
	_stream->Read(buffer, _stream->Size());
	buffer[_stream->Size()] = 0;

	bool ret = _Upload(buffer);
    
    delete[] buffer;
    
    return ret;
}

void CShader::Unload()
{
}
