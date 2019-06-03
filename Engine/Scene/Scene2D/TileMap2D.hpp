#pragma once

#include "Scene/Scene2D/Object2D.hpp"
#include "Core/Map.hpp"

struct CTile
{
	s32	   I;
	s32	   J;
	f32	   X;
	f32	   Y;

	u32	   Flags;
	u32    ID;
	CColor Color;

	f32			G;
	f32			H;
	CTile*		Parent;
};

class CTileMap2D: public CObject2D
{
public:
	enum ETilemapType
	{
		EMT_ORTHOGONAL = 0,
		EMT_ISOMETRIC,
		EMT_STAGGERED,
		EMT_HEXAGONAL
	};

	CTileMap2D();
	virtual ~CTileMap2D();

	void Init(ETilemapType _type, u32 _sizeI, u32 _sizeJ, u32 _tileW, u32 _tileH);
	void RegisterTile(u32 _id, s32 _x, s32 _y, u32 _w, u32 _h);
	void ClearTileset();

	CTile* GetTileXY(s32 _x, s32 _y);
	CTile* GetTileIJ(u32 _i, u32 _j);

	u32	   GetTileID(u32 _i, u32 _j);
	void   SetTileID(u32 _i, u32 _j, u32 _id);

	bool CalcPath(u32 _startI, u32 _startJ, u32 _destI, u32 _destJ, u32 _mask, CList<CTile*>& _output);
	int FindTiles(u32 _startI, u32 _startJ, u32 _steps, u32 _id, u32 _mask, CList<CTile*>& _output);

	virtual void OnTransformed()
	{
		m_Transform = m_Node->GetTransform();

		m_Bound = CRect(-m_Center.X, -m_Center.Y, m_Size.X, m_Size.Y);
		m_Transform.TransformAARect(m_Bound);

		if (m_ClipChild && m_Node)
		{
			CClipEvent cevent(m_ClipChild, m_Bound);
			m_Node->HandleEvent(&cevent);
		}

		m_TransformDirty = true;
		m_VertexesDirty = true;

		if (m_CurrentBatch)
		{
			m_CurrentBatch->MarkDirty();
		}
	}

	void SetTileColor(u32 _i, u32 _j, u32 _color)
	{
		if(_j*m_SizeI+_i >= m_SizeI*m_SizeJ)
			return;

		m_TileMap[_j*m_SizeI+_i].Color = CColor(_color);

		m_VertexesDirty = true;

		if(m_CurrentBatch)
		{
			m_CurrentBatch->Update(this);
		}
	}

	void SetTileFlags(u32 _i, u32 _j, u32 _flags)
	{
		if(_j*m_SizeI+_i >= m_SizeI*m_SizeJ)
			return;

		m_TileMap[_j*m_SizeI+_i].Flags = _flags;
	}

	void _UpdateVertexes();

private:
	u32		_GetNeiborsCount(CTile* _tile);
	CTile*  _GetNeibor(CTile* _tile, u32 _cnt);

	u32			  m_TileW;
	u32			  m_TileH;
	u32			  m_SizeI;
	u32			  m_SizeJ;

	ETilemapType  m_MapType;
	CTile*		  m_TileMap;

	CMap<u32, CRect> m_Tileset;
};