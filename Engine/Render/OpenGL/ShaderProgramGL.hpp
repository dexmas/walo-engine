#include "Render/OpenGL/ShaderGL.hpp"
#include "Render/ShaderParameter.hpp"

#include "Core/Map.hpp"

#include <glew.h>

class CShaderUniformGL
{
public:
	CShaderUniformGL(EShaderParamType _type, u32 _location)
	{
		m_Type = _type;
		m_Location = _location;
	}

	EShaderParamType GetType(){return m_Type;}
	u32				 GetLocation(){return m_Location;}

private:
	EShaderParamType m_Type;
	u32				 m_Location;
};

class CShaderProgramGL
{
public:
	CShaderProgramGL()
	{
		m_VertexShader = 0;
		m_PixelShader = 0;

		m_Handle = glCreateProgram();
	}

	~CShaderProgramGL()
	{
		glDeleteProgram(m_Handle);
	}

	u32 GetHandle() {return m_Handle;}

	CShader* GetPixelShader() {return m_PixelShader;}
	CShader* GetVertexShader() {return m_VertexShader;}

	bool Link(CShader* _vertex, CShader* _pixel)
	{
		m_VertexShader = _vertex;
		m_PixelShader = _pixel;

		// TODO: Bind only exist locations?
		glBindAttribLocation(m_Handle, 0, "iPos");
		glBindAttribLocation(m_Handle, 1, "iNormal");
		glBindAttribLocation(m_Handle, 2, "iColor");
		glBindAttribLocation(m_Handle, 3, "iTexCoord1");
		glBindAttribLocation(m_Handle, 4, "iTexCoord2");
		glBindAttribLocation(m_Handle, 5, "iTexCoord3");
		glBindAttribLocation(m_Handle, 6, "iTexCoord4");
		glBindAttribLocation(m_Handle, 7, "iTangent");

		glAttachShader(m_Handle, m_VertexShader->GetHandle());
		glAttachShader(m_Handle, m_PixelShader->GetHandle());
		glLinkProgram(m_Handle);

		s32 linkStatus;
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &linkStatus);

		glUseProgram(m_Handle);

		const int MAX_PARAMETER_NAME_LENGTH = 256;
		char uniformName[MAX_PARAMETER_NAME_LENGTH];
		s32 uniformCount;
		glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &uniformCount);

		for(s32 i=0;i<uniformCount;i++)
		{
			unsigned type;
			int count;
        
			glGetActiveUniform(m_Handle, i, MAX_PARAMETER_NAME_LENGTH, 0, &count, &type, uniformName);
			s32 location = glGetUniformLocation(m_Handle, uniformName);

			if(CString(uniformName) == "sTexture0")
			{
				glUniform1i(location, 0);
			}
			else
			if(CString(uniformName) == "sTexture1")
			{
				glUniform1i(location, 1);
			}
			else
			if(CString(uniformName) == "sTexture2")
			{
				glUniform1i(location, 2);
			}
			else
			if(CString(uniformName) == "sTexture3")
			{
				glUniform1i(location, 3);
			}
			else
			{
				EShaderParamType sptype = ESPT_COUNT;

				switch(type)
				{
				case GL_FLOAT:
					sptype = ESPT_FLOAT;
					break;
				case GL_FLOAT_VEC2:
					sptype = ESPT_VECTOR2;
					break;
				case GL_FLOAT_VEC3:
					sptype = ESPT_VECTOR3;
					break;
				case GL_FLOAT_MAT4:
					sptype = ESPT_MATRIX4;
					break;
				default:
					break;
				};

				if(sptype != ESPT_COUNT)
				{
					m_UniformLoacations.Insert(CString(uniformName), CShaderUniformGL(sptype, location));
				}
			}
		}

		return (linkStatus == GL_TRUE);
	}

	bool SetShaderParameter(CString& _name, EShaderParamType _type, void* _param)
	{
		if(m_UniformLoacations.Empty())
			return false;

		CMap<CString, CShaderUniformGL>::TNode* node = m_UniformLoacations.Find(_name);
		if(node)
		{
			CShaderUniformGL ufm = node->Value;
			
			if(ufm.GetType() != _type)
				return false;

			switch(_type)
			{
			case ESPT_FLOAT:
				glUniform1fv(ufm.GetLocation(), 1, (f32*)_param);
				break;
			case ESPT_VECTOR2:
				glUniform2fv(ufm.GetLocation(), 1, (f32*)_param);
				break;
			case ESPT_VECTOR3:
				glUniform3fv(ufm.GetLocation(), 1, (f32*)_param);
				break;
			case ESPT_MATRIX4:
				glUniformMatrix4fv(ufm.GetLocation(), 1, GL_FALSE, (f32*)_param);
				break;
			}
		}

		return true;
	}

private:
	u32 m_Handle;

	CMap<CString, CShaderUniformGL> m_UniformLoacations;

	CShader* m_VertexShader;
	CShader* m_PixelShader;
};