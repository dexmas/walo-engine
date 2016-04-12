#pragma once

#include "Scene/Scene2D/Object2D.hpp"
#include "Scene/Scene2D/Font2D.hpp"

#include "Core/Array.hpp"

enum EHAlignment
{
	EHA_LEFT = 0,
	EHA_CENTER,
	EHA_RIGHT
};

enum EVAlignment
{
	EVA_TOP = 0,
	EVA_CENTER,
	EVA_BOTTOM
};

class CText2D: public CObject2D
{
	struct STextLine
	{
		CWString Text;
		u32		 Width;
	};

public:
	CText2D():CObject2D(ECT_TEXT2D)
	{
		m_Font = 0;
		m_Color = CColor::WHITE;
		m_HAlignment = EHA_LEFT;
		m_VAlignment = EVA_TOP;
		m_Texture = 0;
	}

	virtual ~CText2D()
	{
		int i = 0;
	}

	void SetFont(CFont2D* _font)
	{
		m_Font = _font;

		if(m_Font)
		{
			SetTexture(m_Font->GetTexture());

			if(m_Text.Size() > 0)
			{
				_PreprocessText();
			}
		}
	}

	CFont2D* GetFont()
	{
		return m_Font;
	}

	void SetText(const CString& _text)
	{
		m_Text = UTF8to16(_text);

		if(m_Font && m_Text.Size() > 0)
		{
			_PreprocessText();
		}
	}

	const CString GetText() const
	{
		return UTF16to8(m_Text);
	}

	void SetTextSize(CVector2& _size)
	{
		m_TextSize = _size;

		if(m_Font && m_Text.Size() > 0)
		{
			_PreprocessText();
		}
	}

	CVector2 GetTextSize()
	{
		return m_TextSize;
	}

	void SetAlignment(EHAlignment _hal, EVAlignment _val)
	{
		m_HAlignment = _hal;
		m_VAlignment = _val;

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	void _UpdateVertexes()
	{
		if(!m_Font || !m_TextLines.Size())
			return;

		m_Vertexes.SetUsed(m_GlyphCount << 2);
		m_Positions.SetUsed(m_GlyphCount << 2);

		f32 nXCoord = 0.0f;
		f32 nYCoord = 0.0f;

		CTexture* texture = m_Font->GetTexture();

		u32 glyphCounter =  0;

		CList<STextLine>::CIterator it = m_TextLines.Begin();
		while(it != m_TextLines.End())
		{
			STextLine& line = *it;

			++it;

			for(u32 j = 0; j < line.Text.Size(); j++)
			{
				CFont2D::SGlyph* glyph = m_Font->GetGlyph(line.Text[j]);

				if(glyph)
				{
					f32 tx = f32(glyph->X)/f32(texture->GetWidth());
					f32 ty = f32(glyph->Y)/f32(texture->GetHeight());
					f32 tw = f32(glyph->W)/f32(texture->GetWidth());
					f32 th = f32(glyph->H)/f32(texture->GetHeight());

					f32 fXOffset = f32(glyph->Xo);
					f32 fYOffset = f32(glyph->Yo);

					f32 ox = 0.0f;
					f32 oy = 0.0f;

					if(m_HAlignment == EHA_CENTER)
					{
						ox = (m_TextSize.X - line.Width)/2.0f;
					}
					else
					if(m_HAlignment == EHA_RIGHT)
					{
						ox = m_TextSize.X - line.Width;
					}

					if(m_VAlignment == EVA_CENTER)
					{
						oy = (m_TextSize.Y - m_TextLines.Size()*m_Font->GetSize())/2.0f;
					}
					else
					if(m_VAlignment == EVA_BOTTOM)
					{
						oy = m_TextSize.Y - m_TextLines.Size()*m_Font->GetSize();
					}

					u32 idx = glyphCounter << 2;

					m_Positions[idx + 0].X = nXCoord + fXOffset + ox;
					m_Positions[idx + 0].Y = nYCoord + fYOffset + oy;
					m_Positions[idx + 0].Z = 0.0f;
					m_Vertexes[idx + 0].U = tx;
					m_Vertexes[idx + 0].V = ty;
					m_Vertexes[idx + 0].Color = m_Color;

					m_Positions[idx + 1].X = nXCoord + fXOffset + ox;
					m_Positions[idx + 1].Y = nYCoord + f32(glyph->H) + fYOffset + oy;
					m_Positions[idx + 1].Z = 0.0f;
					m_Vertexes[idx + 1].U = tx;
					m_Vertexes[idx + 1].V = ty + th;
					m_Vertexes[idx + 1].Color = m_Color;

					m_Positions[idx + 2].X = nXCoord + f32(glyph->W) + fXOffset + ox;
					m_Positions[idx + 2].Y = nYCoord + f32(glyph->H) + fYOffset + oy;
					m_Positions[idx + 2].Z = 0.0f;
					m_Vertexes[idx + 2].U = tx + tw;
					m_Vertexes[idx + 2].V = ty + th;
					m_Vertexes[idx + 2].Color = m_Color;

					m_Positions[idx + 3].X = nXCoord + f32(glyph->W) + fXOffset + ox;
					m_Positions[idx + 3].Y = nYCoord + fYOffset + oy;
					m_Positions[idx + 3].Z = 0.0f;
					m_Vertexes[idx + 3].U = tx + tw;
					m_Vertexes[idx + 3].V = ty;
					m_Vertexes[idx + 3].Color = m_Color;

					glyphCounter++;

					nXCoord += glyph->Xa;
				}
				else
					nXCoord += m_Font->GetSize();
			}

			nXCoord = 0.0;
			nYCoord += m_Font->GetSize();
		}
	}

private:
	void _PreprocessText()
	{
		u32 lastSpace = -1;
		u32 lineWidth = 0;
		u32 currWidth = 0;
		u32 lineStart = 0;
		u32 maxWidth = 0;

		m_TextLines.Clear();
		m_GlyphCount = 0;

		for(u32 i = 0; i < m_Text.Size(); i++)
		{
			CFont2D::SGlyph* glyph = m_Font->GetGlyph(m_Text[i]);

			if(m_Text[i] == ' ')
			{
				lastSpace = i;
				lineWidth = currWidth;
			}
		
			if(glyph)
			{
				currWidth += glyph->Xa;
				m_GlyphCount++;
			}

			if(i == (m_Text.Size() - 1))
			{
				STextLine line;
				line.Text = CWString(m_Text.CStr() + lineStart, i - lineStart + 1);
				line.Width = currWidth;

				if(currWidth > maxWidth)
					maxWidth = currWidth;

				m_TextLines.PushBack(line);

				break;
			}

			if(m_Text[i] == '\n')
			{
				STextLine line;
				line.Text = CWString(m_Text.CStr() + lineStart, i - lineStart);
				line.Width = currWidth;

				if(currWidth > maxWidth)
					maxWidth = currWidth;

				m_TextLines.PushBack(line);

				lineWidth = 0;
				currWidth = 0;
				lineStart = i+1;
				lastSpace = -1;

				continue;
			}

			if(!glyph)
			{
				currWidth += m_Font->GetSize();
			}

			if(m_TextSize.X > 0 && currWidth > m_TextSize.X && lineWidth != 0)
			{
				if(lastSpace == -1)
					lastSpace = i;

				STextLine line;
				line.Text = CWString(m_Text.CStr() + lineStart, lastSpace - lineStart);
				line.Width = lineWidth;

				if(lineWidth > maxWidth)
					maxWidth = lineWidth;

				m_TextLines.PushBack(line);

				currWidth = currWidth - lineWidth;
				lineWidth = 0;
				lineStart = lastSpace + 1;
				lastSpace = -1;
			}
		}

		m_Size.X = maxWidth;
		m_Size.Y = m_Font->GetSize() * m_TextLines.Size();

		m_Center.X = m_Size.X - m_TextSize.X;
		m_Center.Y = m_Size.Y - m_TextSize.Y;

		if(m_HAlignment == EHA_CENTER)
		{
			m_Center.X /= 2.0f;
		}

		if(m_VAlignment == EVA_CENTER)
		{
			m_Center.Y /= 2.0f;
		}

		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->MarkDirty();
		}
	}

	CFont2D*  m_Font;
	CWString  m_Text;

	EHAlignment m_HAlignment;
	EVAlignment m_VAlignment;

	CVector2 m_TextSize;

	CList<STextLine> m_TextLines;
	u32				 m_GlyphCount;
};