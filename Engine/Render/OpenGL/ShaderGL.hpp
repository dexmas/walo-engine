#pragma once

#include "Render/Shader.hpp"

class CShaderGL: public CShader
{
public:
	CShaderGL(EShaderType _type);
	~CShaderGL();

	bool _Upload(const char* _data);
};
