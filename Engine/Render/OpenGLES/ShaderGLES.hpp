#pragma once

#include "Render/Shader.hpp"

class CShaderGLES: public CShader
{
public:
	CShaderGLES(EShaderType _type);
	~CShaderGLES();

	bool _Upload(void* _data);
};