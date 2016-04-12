#pragma once

#include "Scene/Scene2D/Object2D.hpp"

class CSprite2D: public CObject2D
{
public:

	CSprite2D():CObject2D(ECT_SPRITE2D)
	{
		m_TextureRect = CRect(0,0,0,0);

		m_Texture = 0;

		m_TilingX = false;
		m_TilingY = false;
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

	void SetTiling(bool _tx, bool _ty)
	{
		m_TilingX = _tx;
		m_TilingY = _ty;

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	void _UpdateVertexes()
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
			f32 tiling_kx = 1.0f;
			f32 tiling_ky = 1.0f;

			if(m_TilingX && m_TextureRect.Width != 0)
				tiling_kx = m_Size.X / m_TextureRect.Width;
			if(m_TilingY && m_TextureRect.Height != 0)
				tiling_ky = m_Size.Y / m_TextureRect.Height;

			texx = f32(m_TextureRect.X)/f32(m_Texture->GetWidth());
			texy = f32(m_TextureRect.Y)/f32(m_Texture->GetHeight());
			texw = f32(m_TextureRect.Width)/f32(m_Texture->GetWidth()) * tiling_kx;
			texh = f32(m_TextureRect.Height)/f32(m_Texture->GetHeight()) * tiling_ky;
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

		m_Positions[2].X = m_Size.X - m_Center.X;
		m_Positions[2].Y = m_Size.Y - m_Center.Y;
		m_Positions[2].Z = 0.0f;
		m_Vertexes[2].U = flipX ? texx : texx + texw;
		m_Vertexes[2].V = flipY ? texy : texy + texh;
		m_Vertexes[2].Color = m_Color;

		m_Positions[3].X = m_Size.X - m_Center.X;
		m_Positions[3].Y = -m_Center.Y;
		m_Positions[3].Z = 0.0f;
		m_Vertexes[3].U = flipX ? texx : texx + texw;
		m_Vertexes[3].V = flipY ? texy + texh : texy;
		m_Vertexes[3].Color = m_Color;
	}

protected:
	CRect m_TextureRect;

	bool m_TilingX;
	bool m_TilingY;
};