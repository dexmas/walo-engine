#include "Scene/DebugRenderer.hpp"
#include "Scene/Scene3D/StaticModel.hpp"

#include "Game.hpp"
#include "Render/Render.hpp"

#include "Assets/AssetsDB.hpp"

CDebugRenderer* CDebugRenderer::m_Instance = 0;

struct SDebugRenderVertex
{
	f32 X;
	f32 Y;
	f32 Z;
	u32 C;
};

CDebugRenderer::CDebugRenderer()
{
	m_LineList.Reallocate(1024);
	m_TriangleList.Reallocate(1024);

	m_Material = CAssetsDB::Instance()->GetMaterial("debugR.mat");
}

void CDebugRenderer::AddBBox(const CBBox& _box, const CColor& _color)
{
	CVector3 sz = _box.Size();

	CVector3 p1 = _box.Min();
	CVector3 p2 = _box.Min() + CVector3(0,0,sz.Z);
	CVector3 p3 = _box.Min() + CVector3(sz.X,0,sz.Z);
	CVector3 p4 = _box.Min() + CVector3(sz.X,0,0);
	CVector3 p5 = _box.Min() + CVector3(0,sz.Y,0);
	CVector3 p6 = _box.Min() + CVector3(0,sz.Y,sz.Z);
	CVector3 p7 = _box.Max();
	CVector3 p8 = _box.Min() + CVector3(sz.X,sz.Y,0);

	m_LineList.Insert(SDebugRendererLine(p1, p2, _color));
	m_LineList.Insert(SDebugRendererLine(p2, p3, _color));
	m_LineList.Insert(SDebugRendererLine(p3, p4, _color));
	m_LineList.Insert(SDebugRendererLine(p4, p1, _color));
	m_LineList.Insert(SDebugRendererLine(p5, p6, _color));
	m_LineList.Insert(SDebugRendererLine(p6, p7, _color));
	m_LineList.Insert(SDebugRendererLine(p7, p8, _color));
	m_LineList.Insert(SDebugRendererLine(p8, p5, _color));
	m_LineList.Insert(SDebugRendererLine(p1, p5, _color));
	m_LineList.Insert(SDebugRendererLine(p2, p6, _color));
	m_LineList.Insert(SDebugRendererLine(p3, p7, _color));
	m_LineList.Insert(SDebugRendererLine(p4, p8, _color));
}

void CDebugRenderer::AddRect(const CRect& _rect, const CColor& _color)
{
	CVector3 p1 = CVector3(_rect.X, _rect.Y, 0.0f);
	CVector3 p2 = CVector3(_rect.X + _rect.Width, _rect.Y, 0.0f);
	CVector3 p3 = CVector3(_rect.X, _rect.Y + _rect.Height, 0.0f);
	CVector3 p4 = CVector3(_rect.X + _rect.Width, _rect.Y + _rect.Height, 0.0f);

	m_LineList.Insert(SDebugRendererLine(p1, p2, _color));
	m_LineList.Insert(SDebugRendererLine(p2, p4, _color));
	m_LineList.Insert(SDebugRendererLine(p4, p3, _color));
	m_LineList.Insert(SDebugRendererLine(p3, p1, _color));
}

void CDebugRenderer::AddLine(const CVector3& _start, const CVector3& _end, const CColor& _color)
{
	m_LineList.Insert(SDebugRendererLine(_start, _end, _color));
}

void CDebugRenderer::AddCircle(const CVector3& _center, f32 _radius, const CColor& _color)
{
    for(u32 i = 0; i < 360; i += 30)
    {
        u32 j = i + 30;
        float x1 = _radius * CMath::Cos((float)i);
        float y1 = _radius * CMath::Sin((float)i);
        float x2 = _radius * CMath::Cos((float)j);
        float y2 = _radius * CMath::Sin((float)j);

		m_LineList.Insert(SDebugRendererLine(_center + CVector3(x1, y1, 0.0f), _center + CVector3(x2, y2, 0.0f), _color));
    }
}

void CDebugRenderer::AddTriangle(const CVector3& _v1, const CVector3& _v2, const CVector3& _v3, const CColor& _color)
{
	m_TriangleList.Insert(SDebugRendererTriangle(_v1, _v2, _v3, _color));
}

void CDebugRenderer::Render()
{
	CRender* rndr = CGame::Instance()->GetRender();

	CVertexBuffer* vbuf = rndr->CreateVertexBuffer();

	vbuf->SetSize(m_LineList.Size() * 2 + m_TriangleList.Size() * 3, EEM_POSITION|EEM_COLOR);
	
	for(u32 i = 0; i < m_LineList.Size(); i++)
	{
		SDebugRendererLine& line = m_LineList[i];

		SDebugRenderVertex v[2];
		v[0].X = line.Start.X;
		v[0].Y = line.Start.Y;
		v[0].Z = line.Start.Z;
		v[0].C = line.Color;

		v[1].X = line.End.X;
		v[1].Y = line.End.Y;
		v[1].Z = line.End.Z;
		v[1].C = line.Color;

		vbuf->SetData(v, i * 2, 2);
	}

	for(u32 i = 0; i < m_TriangleList.Size(); i++)
	{
		SDebugRendererTriangle& tri = m_TriangleList[i];

		SDebugRenderVertex v[3];
		v[0].X = tri.V1.X;
		v[0].Y = tri.V1.Y;
		v[0].Z = tri.V1.Z;
		v[0].C = tri.Color;

		v[1].X = tri.V2.X;
		v[1].Y = tri.V2.Y;
		v[1].Z = tri.V2.Z;
		v[1].C = tri.Color;

		v[2].X = tri.V3.X;
		v[2].Y = tri.V3.Y;
		v[2].Z = tri.V3.Z;
		v[2].C = tri.Color;

		vbuf->SetData(v, m_LineList.Size() * 2 + i * 3, 3);
	}

	rndr->SetMaterial(m_Material);
	rndr->SetVertexBuffer(vbuf);

	rndr->Render(EPT_LINES, 0, m_LineList.Size() * 2);
	rndr->Render(EPT_TRIANGLES, m_LineList.Size() * 2, m_TriangleList.Size() * 3);

	delete vbuf;

	m_LineList.SetUsed(0);
	m_TriangleList.SetUsed(0);
}