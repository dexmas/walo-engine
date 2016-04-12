#pragma once

#include "Core/Color.hpp"
#include "Core/Vector.hpp"
#include "Core/Array.hpp"
#include "Core/BBox.hpp"
#include "Core/Rect.hpp"

class CRender;
class CStaticModel;
class CMaterial;

struct SDebugRendererLine
{
	SDebugRendererLine(const CVector3& _start, const CVector3& _end, const CColor& _color)
	{
		Start = _start;
		End = _end;
		Color = _color;
	}

	CVector3 Start;
	CVector3 End;
	CColor   Color;
};

struct SDebugRendererTriangle
{
	SDebugRendererTriangle(const CVector3& _v1, const CVector3& _v2, const CVector3& _v3, const CColor& _color)
	{
		V1 = _v1;
		V2 = _v2;
		V3 = _v3;
		Color = _color;
	}

	CVector3 V1;
	CVector3 V2;
	CVector3 V3;
	CColor   Color;
};

class CDebugRenderer
{
public:
	CDebugRenderer();

	void AddBBox(const CBBox& _box, const CColor& _color);
	void AddRect(const CRect& _rect, const CColor& _color);
	void AddLine(const CVector3& _start, const CVector3& _end, const CColor& _color);
	void AddCircle(const CVector3& _center, f32 _radius, const CColor& _color);
	void AddTriangle(const CVector3& _v1, const CVector3& _v2, const CVector3& _v3, const CColor& _color);

	void Render();

	static CDebugRenderer* Instance()
	{
		if(!m_Instance)
		{
			m_Instance = new CDebugRenderer();
		}

		return m_Instance;
	}

private:
	static CDebugRenderer* m_Instance;

	CPODArray<SDebugRendererLine> m_LineList;
	CPODArray<SDebugRendererTriangle> m_TriangleList;
	CMaterial* m_Material;
};