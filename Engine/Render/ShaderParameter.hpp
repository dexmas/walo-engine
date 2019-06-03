#pragma once

#include "Core/Types.hpp"
#include "Core/Vector.hpp"
#include "Core/Matrix.hpp"
#include "Render/RenderTypes.hpp"

class CShaderParameter
{
public:
	CShaderParameter(EShaderParamType _type)
	{
		m_Type = _type;

		m_FloatValue = 0.0f;
		m_Vector2Value = CVector2(0.0f, 0.0f);
		m_Vector3Value = CVector3(0.0f, 0.0f, 0.0f);
		m_Matrix4Value = CMatrix4::IDENTY;
	}

	CShaderParameter(float _value)
	{
        m_Type = ESPT_FLOAT;
        
        m_FloatValue = _value;
        m_Vector2Value = CVector2(0.0f, 0.0f);
        m_Vector3Value = CVector3(0.0f, 0.0f, 0.0f);
        m_Matrix4Value = CMatrix4::IDENTY;
    }

	CShaderParameter(const CVector2& _value)
	{
        m_Type = ESPT_VECTOR2;
        
        m_FloatValue = 0.0f;
        m_Vector2Value = _value;
        m_Vector3Value = CVector3(0.0f, 0.0f, 0.0f);
        m_Matrix4Value = CMatrix4::IDENTY;
    }

	CShaderParameter(const CVector3& _value)
	{
        m_Type = ESPT_VECTOR3;
        
        m_FloatValue = 0.0f;
        m_Vector2Value = CVector2(0.0f, 0.0f);
        m_Vector3Value = _value;
        m_Matrix4Value = CMatrix4::IDENTY;
    }

	CShaderParameter(const CMatrix4& _value)
	{
        m_Type = ESPT_MATRIX4;
        
        m_FloatValue = 0.0f;
        m_Vector2Value = CVector2(0.0f, 0.0f);
        m_Vector3Value = CVector3(0.0f, 0.0f, 0.0f);
        m_Matrix4Value = _value;
    }

	EShaderParamType& GetType(){return m_Type;}

	void* GetValue()
	{
		switch(m_Type)
		{
		case ESPT_FLOAT:
			return &m_FloatValue;
			break;
		case ESPT_VECTOR2:
			return &m_Vector2Value.X;
			break;
		case ESPT_VECTOR3:
			return &m_Vector3Value.X;
			break;
		case ESPT_MATRIX4:
			return &m_Matrix4Value.M;
			break;
        default:
                return 0;
		}

		return 0;
	}

	f32&	  Float()  {return m_FloatValue;}
	CVector2& Vector2(){return m_Vector2Value;}
	CVector3& Vector3(){return m_Vector3Value;}
	CMatrix4& Matrix4(){return m_Matrix4Value;}

private:
	EShaderParamType m_Type;

	f32		 m_FloatValue;
	CVector2 m_Vector2Value;
	CVector3 m_Vector3Value;
	CMatrix4 m_Matrix4Value;
};
