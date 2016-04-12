#pragma once

#include "Scene/Scene2D/Object2D.hpp"

class CProgress2D: public CObject2D
{
public:
	enum EProgressType
	{
		EPT_LINEAR = 0,
		EPT_RADIAL
	};

	CProgress2D():CObject2D(ECT_PROGRESS2D)
	{
		m_TextureRect = CRect(0,0,0,0);
		m_Texture = 0;
		m_Type = EPT_LINEAR;
		m_Value = 1.0f;
	}

	const CRect& GetFrame() const
	{
		return m_TextureRect;
	}

	void SetFrame(const CRect& _rect)
	{
		m_TextureRect = _rect;

		SetSize(CVector2(_rect.Width, _rect.Height));

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	const EProgressType GetType() const
	{
		return m_Type;
	}

	void SetType(EProgressType _type)
	{
		m_Type = _type;
		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	f32 GetValue()
	{
		return m_Value;
	}

	void SetValue(f32 _val)
	{
		m_Value = _val;
		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	void _UpdateLinear()
	{
		m_Vertexes.SetUsed(4);
		m_Positions.SetUsed(4);

		bool flipX = m_Flip.X > 0.0f;
		bool flipY = m_Flip.Y > 0.0f;

		f32 texx = 0.0f;
		f32 texy = 0.0f;
		f32 texw = 0.0f;
		f32 texh = 0.0f;

		if(m_Texture)
		{
			texx = f32(m_TextureRect.X)/f32(m_Texture->GetWidth());
			texy = f32(m_TextureRect.Y)/f32(m_Texture->GetHeight());
			texw = m_Value * f32(m_TextureRect.Width)/f32(m_Texture->GetWidth());
			texh = m_Value * f32(m_TextureRect.Height)/f32(m_Texture->GetHeight());
		}
		
		m_Positions[0].X = -m_Center.X;
		m_Positions[0].Y = -m_Center.Y;
		m_Positions[0].Z = 0.0f;
		m_Vertexes[0].U = flipX ? texx + texw : texx;
		m_Vertexes[0].V = flipY ? texy + texh : texy;
		m_Vertexes[0].Color = m_Color;
		
		m_Positions[1].X = -m_Center.X;
		m_Positions[1].Y = m_Size.Y - m_Center.Y;
		m_Positions[1].Z = 0.0f;
		m_Vertexes[1].U = flipX ? texx + texw : texx;
		m_Vertexes[1].V = flipY ? texy : texy + texh;
		m_Vertexes[1].Color = m_Color;

		m_Positions[2].X = m_Size.X * m_Value - m_Center.X;
		m_Positions[2].Y = m_Size.Y - m_Center.Y;
		m_Positions[2].Z = 0.0f;
		m_Vertexes[2].U = flipX ? texx : texx + texw;
		m_Vertexes[2].V = flipY ? texy : texy + texh;
		m_Vertexes[2].Color = m_Color;

		m_Positions[3].X = m_Size.X * m_Value - m_Center.X;
		m_Positions[3].Y = -m_Center.Y;
		m_Positions[3].Z = 0.0f;
		m_Vertexes[3].U = flipX ? texx : texx + texw;
		m_Vertexes[3].V = flipY ? texy + texh : texy;
		m_Vertexes[3].Color = m_Color;
	}

	float _Angle(const CVector2& _a, const CVector2& _b) const
    {
		CVector2 na = _a;
		CVector2 nb = _b;

        na.Normalize(); 
		nb.Normalize();

        return acosf(na.Dot(nb));
    }

	void _Rotate(CVector2& _v, float _rad)
    {
        CVector2 vh;

        vh.X = _v.X * cosf(_rad) - _v.Y * sinf(_rad);
        vh.Y = _v.X * sinf(_rad) + _v.Y * cosf(_rad);

        _v = vh;
    }

	void _UpdateRadial()
	{
		m_Vertexes.SetUsed(12);
		m_Positions.SetUsed(12);

		for(u32 i = 0; i < 12; i++)
		{
			m_Vertexes[i].Color = m_Color;
		}

		bool flipX = m_Flip.X > 0.0f;
		bool flipY = m_Flip.Y > 0.0f;

		m_Center = m_Size/2.0f;

		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		f32 texx = 0.0f;
		f32 texy = 0.0f;
		f32 texw = 0.0f;
		f32 texh = 0.0f;

		if(m_Texture)
		{
			texx = f32(m_TextureRect.X)/f32(m_Texture->GetWidth());
			texy = f32(m_TextureRect.Y)/f32(m_Texture->GetHeight());
			texw = f32(m_TextureRect.Width)/f32(m_Texture->GetWidth());
			texh = f32(m_TextureRect.Height)/f32(m_Texture->GetHeight());
		}

		CVector2 vdiag1 = CVector2(-m_Center.X + m_Size.X, -m_Center.Y);
		CVector2 vdiag2 = CVector2(-m_Center.X + m_Size.X, -m_Center.Y + m_Size.Y);

		CVector2 vecRad(0.0f, -vdiag1.Length());

		f32 angle = CMath::TWO_PI * (flipX? 1.0f - m_Value : m_Value);

		f32 fa1 = _Angle(vdiag1, vecRad);
		f32 fa2 = _Angle(vdiag2, vdiag1);

		_Rotate(vecRad, angle);

		f32 angles[6];
		angles[0] = fa1;
		angles[1] = fa2;
		angles[2] = fa1;
		angles[3] = fa1;
		angles[4] = fa2;
		angles[5] = fa1;

        for(u32 i = 0; i < 6; i++)
        {
			f32 result = 0.0f;
            f32 limitLo = 0.0f;
            f32 limitHi = 0.0f;

            for(u32 j = 0; j < i; j++)
                limitLo += angles[j];

            limitHi = limitLo + angles[ i ];

            bool bOverHi = angle > limitHi;
            bool bOverLo = angle < limitLo;

            switch(i)
            {
                case 0:
                {
                    result = bOverHi ? (-m_Center.X + m_Size.X) : vecRad.X;

					m_Positions[0] = CVector3(0.0f, 0.0f, 0.0f);
					m_Vertexes[0].U = texx + texw / 2.0f;
					m_Vertexes[0].V = texy + texh / 2.0f;

					if(bOverLo)
					{
						m_Positions[1] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[1].U = texx + texw / 2.0f;
						m_Vertexes[1].V = texy + texh / 2.0f;

						m_Positions[2] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[2].U = texx + texw / 2.0f;
						m_Vertexes[2].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[1] = CVector3(0.0, -m_Center.Y, 0.0f);
						m_Vertexes[1].U = texx + texw / 2.0f;
						m_Vertexes[1].V = texy;

						m_Positions[2] = CVector3(result, -m_Center.Y, 0.0f);
						m_Vertexes[2].U = texx + (texw / 2.0f) + (texw / 2.0f) * (result/(-m_Center.X + m_Size.X));
						m_Vertexes[2].V = texy;
					}
                }
                break;
                case 1:
                {
                    result = bOverHi ? (-m_Center.Y + m_Size.Y) : vecRad.Y;

					if(bOverLo)
					{
						m_Positions[3] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[3].U = texx + texw / 2.0f;
						m_Vertexes[3].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[3] = CVector3(-m_Center.X + m_Size.X, result, 0.0f);
						m_Vertexes[3].U = texx + texw;
						m_Vertexes[3].V = texy + (texh / 2.0f) + (texh / 2.0f) * (result/(-m_Center.Y + m_Size.Y));
					}
                }
                break;
                case 2:
                {
                    result = bOverHi ? 0.f : vecRad.X;

                    m_Positions[4] = CVector3(0.0f, 0.0f, 0.0f);
					m_Vertexes[4].U = texx + texw / 2.0f;
					m_Vertexes[4].V = texy + texh / 2.0f;

					if(bOverLo)
					{
						m_Positions[5] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[5].U = texx + texw / 2.0f;
						m_Vertexes[5].V = texy + texh / 2.0f;

						m_Positions[6] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[6].U = texx + texw / 2.0f;
						m_Vertexes[6].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[5] = CVector3(-m_Center.X + m_Size.X, -m_Center.Y + m_Size.Y, 0.0f);
						m_Vertexes[5].U = texx + texw;
						m_Vertexes[5].V = texy + texh;

						m_Positions[6] = CVector3(result, -m_Center.Y + m_Size.Y, 0.0f);
						m_Vertexes[6].U = texx + (texw / 2.0f) + (texw / 2.0f) * (result/(-m_Center.X + m_Size.X));
						m_Vertexes[6].V = texy + texh;
					}
                }
                break;
                case 3:
                {
                    result = bOverHi ? -m_Center.X : vecRad.X;

					if(bOverLo)
					{
						m_Positions[7] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[7].U = texx + texw / 2.0f;
						m_Vertexes[7].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[7] = CVector3(result, -m_Center.Y + m_Size.Y, 0.0f);
						m_Vertexes[7].U = texx + (texw / 2.0f) * (1.0f - result/-m_Center.X);
						m_Vertexes[7].V = texy + texh;
					}
                }
                break;
                case 4:
                {
                    result = bOverHi ? -m_Center.Y : vecRad.Y;

                    m_Positions[8] = CVector3(0.0f, 0.0f, 0.0f);
					m_Vertexes[8].U = texx + texw / 2.0f;
					m_Vertexes[8].V = texy + texh / 2.0f;

					if(bOverLo)
					{
						m_Positions[9] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[9].U = texx + texw / 2.0f;
						m_Vertexes[9].V = texy + texh / 2.0f;

						m_Positions[10] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[10].U = texx + texw / 2.0f;
						m_Vertexes[10].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[9] = CVector3(-m_Center.X, -m_Center.Y + m_Size.Y, 0.0f);
						m_Vertexes[9].U = texx;
						m_Vertexes[9].V = texy + texh;

						m_Positions[10] = CVector3(-m_Center.X, result, 0.0f);
						m_Vertexes[10].U = texx;
						m_Vertexes[10].V = texy + (texh / 2.0f) - (texh / 2.0f) * (result/-m_Center.Y);
					}
                }
                break;
                case 5:
                {
                    result = bOverHi ? 0.0f : vecRad.X;

					if(bOverLo)
					{
						m_Positions[11] = CVector3(0.0f, 0.0f, 0.0f);
						m_Vertexes[11].U = texx + texw / 2.0f;
						m_Vertexes[11].V = texy + texh / 2.0f;
					}
					else
					{
						m_Positions[11] = CVector3(result, -m_Center.Y, 0.0f);
						m_Vertexes[11].U = texx + (texw / 2.0f) * (1.0f - result/-m_Center.X);
						m_Vertexes[11].V = texy;
					}
                }
                break;
                default:
                    continue;
            }
		}
	}

	void _UpdateVertexes()
	{
		switch(m_Type)
		{
		case EPT_RADIAL:
			_UpdateRadial();
			break;
		default:
			_UpdateLinear();
			break;
		}
	}

protected:
	CRect m_TextureRect;
	EProgressType m_Type;
	f32 m_Value;
};