#include "Scene/Scene2D/TileMap2D.hpp"
#include "Game.hpp"

CTileMap2D::CTileMap2D():CObject2D(ECT_TILEMAP2D)
{
	m_MapType = EMT_ORTHOGONAL;

	m_TileW = 0;
	m_TileH = 0;
	m_SizeI = 0;
	m_SizeJ = 0;

	m_TileMap = 0;
}

CTileMap2D::~CTileMap2D()
{
	if(m_TileMap)
	{
		delete[] m_TileMap;
	}
}

void CTileMap2D::Init(ETilemapType _type, u32 _sizeI, u32 _sizeJ, u32 _tileW, u32 _tileH)
{
	m_MapType = _type;

	m_TileW = _tileW;
	m_TileH = _tileH;

	m_SizeI = _sizeI;
	m_SizeJ = _sizeJ;

	CVector2 size(0.0f, 0.0f);
		
	if(m_TileMap)
	{
		delete[] m_TileMap;
	}

	m_TileMap = new CTile[m_SizeI*m_SizeJ];

	for(u32 i=0; i< m_SizeI*m_SizeJ; i++)
	{
		m_TileMap[i].Color = CColor::WHITE;

		m_TileMap[i].ID = 0;
		m_TileMap[i].Flags = 0;
		m_TileMap[i].I = i % m_SizeI;
		m_TileMap[i].J = i / m_SizeI;

		m_TileMap[i].G = 0.0f;
		m_TileMap[i].H = 0.0f;
		m_TileMap[i].Parent = 0;

		if(m_MapType == EMT_ORTHOGONAL)
		{
			m_TileMap[i].X = f32(m_TileW * m_TileMap[i].I);
			m_TileMap[i].Y = f32(m_TileH * m_TileMap[i].J);

			size.X = m_TileW * m_SizeI;
			size.Y = m_TileH * m_SizeJ;
		}
		else
		if(m_MapType == EMT_ISOMETRIC)
		{
			m_TileMap[i].X = f32(m_TileW * m_TileMap[i].I);
			m_TileMap[i].Y = f32(m_TileH * m_TileMap[i].J);
			if(m_TileMap[i].J%2)
				m_TileMap[i].X += f32(m_TileH)/2.0f;
		}
		else
		if(m_MapType == EMT_STAGGERED)
		{
			m_TileMap[i].X = f32(m_TileW * m_TileMap[i].I);
			m_TileMap[i].Y = f32(m_TileH * m_TileMap[i].J);

			size.X = m_TileW * m_SizeI;
			size.Y = m_TileH * m_SizeJ;
		}
		else
		if(m_MapType == EMT_HEXAGONAL)
		{
			m_TileMap[i].X = f32((m_TileW * 0.75f) * m_TileMap[i].I);
			m_TileMap[i].Y = f32(m_TileH * m_TileMap[i].J);

			if(m_TileMap[i].I%2)
				m_TileMap[i].Y += f32(m_TileH)/2.0f;

			size.X = (m_TileW  * 0.75f) * m_SizeI + (m_TileW  * 0.25f);
			size.Y = (m_TileH * m_SizeJ) + m_TileH * 0.5f;
		}
	}

	SetSize(size);
}

void CTileMap2D::RegisterTile(u32 _id, s32 _x, s32 _y, u32 _w, u32 _h)
{
	m_Tileset.Insert(_id, CRect(_x,_y, _w, _h));
}

void CTileMap2D::ClearTileset()
{
	m_Tileset.Clear();
}

u32	CTileMap2D::GetTileID(u32 _i, u32 _j)
{
	if(_j*m_SizeI+_i >= m_SizeI*m_SizeJ)
		return 0;

	return m_TileMap[_j*m_SizeI+_i].ID;
}

void CTileMap2D::SetTileID(u32 _i, u32 _j, u32 _id)
{
	if(_j*m_SizeI+_i >= m_SizeI*m_SizeJ)
		return;

	m_TileMap[_j*m_SizeI+_i].ID = _id;

	m_VertexesDirty = true;

	if(m_CurrentBatch)
	{
		m_CurrentBatch->MarkDirty();
	}
}

u32 CTileMap2D::_GetNeiborsCount(CTile* _tile)
{
	if(m_MapType == EMT_HEXAGONAL)
	{
		return 6;
	}
	else
		return 4;
}

CTile* CTileMap2D::_GetNeibor(CTile* _tile, u32 _cnt)
{
	int walkTempl[8][2]={{1,0},{0,1},{-1,0},{0,-1},{1,1},{-1,1},{-1,-1},{1,-1}};

	if(m_MapType == EMT_HEXAGONAL)
	{
		if((_tile->I % 2) == 0)
		{
			walkTempl[4][0] = 1;
			walkTempl[4][1] = -1;

			walkTempl[5][0] = -1;
			walkTempl[5][1] = -1;
		}
		else
		{
			walkTempl[4][0] = 1;
			walkTempl[4][1] = 1;

			walkTempl[5][0] = -1;
			walkTempl[5][1] = 1;
		}
	}

	return GetTileIJ(_tile->I + walkTempl[_cnt][0], _tile->J + walkTempl[_cnt][1]);
}

f32 _heuristic(CTile* _from, CTile* _to)
{
	 f32 dx = abs(_from->X - _to->X);
	 f32 dy = abs(_from->Y - _to->Y);

	 return dx + dy;
}

//#TODO: Optimize (remove close list)
bool CTileMap2D::CalcPath(u32 _startI, u32 _startJ, u32 _destI, u32 _destJ, u32 _mask, CList<CTile*>& _output)
{
	_output.Clear();

	CTile* startT = GetTileIJ(_startI, _startJ);
	CTile* destT = GetTileIJ(_destI, _destJ);

	if(!startT || !destT || ((destT->Flags & _mask) != _mask))
	{
		return false;
	}

	for(u32 i=0; i< m_SizeI*m_SizeJ; i++)
	{
		m_TileMap[i].G = 0.0f;
		m_TileMap[i].H = 0.0f;
		m_TileMap[i].Parent = 0;
	}

	CList<CTile*> openList;
	CList<CTile*> closeList;
	CTile* current;

	openList.PushBack(startT);
	bool pathFound = false;
        
    while(!openList.Empty())
    {
		CList<CTile*>::CIterator it = openList.Begin();
		current = *it;

		while(++it != openList.End())
		{
			CTile* tile = *it;

			f32 currCost = current->G + current->H;
			f32 tileCost = tile->G + tile->H;

			if(tileCost <= currCost)
			{
				current = tile;
			}
		}

		if(current == destT)
		{
			pathFound = true;
			break;
		}

		closeList.PushBack(current);
		openList.Erase(current);
            
        for(u32 i=0;i<_GetNeiborsCount(current);i++)
        {
            CTile* nextTile = _GetNeibor(current, i);

            if((nextTile) && ((nextTile->Flags & _mask) == _mask))
            {
				if(closeList.Find(nextTile) != closeList.End())
				{
					continue;
				}

				f32 totalCost = current->G + ((i < 4) ? 1.0f : 1.4f);

                CList<CTile*>::CIterator jt = openList.Find(nextTile);
				if (jt == openList.End()) 
				{
					nextTile->Parent = current;
					nextTile->G = totalCost;
					nextTile->H = _heuristic(nextTile, destT);
					openList.PushBack(nextTile);
				}
				else 
				if (totalCost < nextTile->G) 
				{
					nextTile->Parent = current;
					nextTile->G = totalCost;
				}
            }
        }
    }

	if(pathFound)
	{
		while(current != 0) 
		{
			if(current != startT)
			{
				_output.PushFront(current);
			}

			current = current->Parent;
		}

		return true;
	}

	return false;
}

//#TODO: Optimize (remove close list)
int CTileMap2D::FindTiles(u32 _startI, u32 _startJ, u32 _steps, u32 _id, u32 _mask, CList<CTile*>& _output)
{
	_output.Clear();

	CTile* startT = GetTileIJ(_startI, _startJ);

	if (!startT)
	{
		return 0;
	}

	CList<CTile*> openList;
	CList<CTile*> closeList;
	CTile* current;

	startT->H = _steps;

	openList.PushBack(startT);

	while (!openList.Empty())
	{
		CList<CTile*>::CIterator it = openList.Begin();
		current = *it;

		openList.Erase(current);

		for (u32 i = 0; i < _GetNeiborsCount(current); i++)
		{
			CTile* nextTile = _GetNeibor(current, i);

			if (nextTile)
			{
				if (closeList.Find(nextTile) == closeList.End())
				{
					if ((nextTile->Flags & _mask) == _mask && (_id != -1 ? nextTile->ID == _id : true))
					{
						closeList.PushBack(nextTile);
						nextTile->H = current->H - 1.0;

						_output.PushBack(nextTile);

						if (nextTile->H > 0)
						{
							openList.PushBack(nextTile);
						}
					}
				}
			}
		}
	}

	return _output.Size();
}

void CTileMap2D::_UpdateVertexes()
{
	if(!m_TileMap)
		return;

	const CVector2& scrsz = CGame::Instance()->GetDevice()->GetScreenSize();
	const CVector3& scale = m_Node->GetScale();
	const CVector3& pos = m_Node->GetPosition();

	f32 wK = 1.0;
	f32 hK = 1.0;

	if (m_MapType == EMT_HEXAGONAL)
	{
		wK = 0.75;
	}

	s32 startI = CMath::Max(0, s32(-pos.X / (m_TileW * wK * scale.X)));
	s32 startJ = CMath::Max(0, s32(-pos.Y / (m_TileH * hK * scale.Y)));

	s32 finI = CMath::Min(m_SizeI, 1 + s32((-pos.X + scrsz.X)/(m_TileW * wK * scale.X)));
	s32 finJ = CMath::Min(m_SizeJ, 1 + s32((-pos.Y + scrsz.Y)/(m_TileH * hK * scale.Y)));

	u32 tileCount = (finI + 1 - startI) * (finJ + 1 - startJ);
	u32 currTile = 0;

	m_Vertexes.SetUsed(tileCount << 2);
	m_Positions.SetUsed(tileCount << 2);

	for(u32 i = startI; i < finI; i++)
	{
		for(u32 j = startJ; j < finJ; j++)
		{
			CTile& tile = m_TileMap[j*m_SizeI + i];

			u32 id   = tile.ID;
			CColor& clr = tile.Color;

			CRect rc = (m_Tileset.Find(id))?m_Tileset[id]:CRect();

			f32 tx = 0.0f;
			f32 ty = 0.0f;
			f32 tw = 0.0f;
			f32 th = 0.0f;

			if(m_Texture)
			{
				tx = f32(rc.X)/f32(m_Texture->GetWidth());
				ty = f32(rc.Y)/f32(m_Texture->GetHeight());
				tw = f32(rc.Width)/f32(m_Texture->GetWidth());
				th = f32(rc.Height)/f32(m_Texture->GetHeight());
			}

			f32 x = tile.X;
			f32 y = tile.Y;

			u32 idx = currTile << 2;

			m_Positions[idx + 0].X = x;
			m_Positions[idx + 0].Y = y + m_TileH;
			m_Positions[idx + 0].Z = 0.0f;
			m_Vertexes[idx + 0].U = tx;
			m_Vertexes[idx + 0].V = ty + th;
			m_Vertexes[idx + 0].Color = clr;

			m_Positions[idx + 1].X = x;
			m_Positions[idx + 1].Y = y;
			m_Positions[idx + 1].Z = 0.0f;
			m_Vertexes[idx + 1].U = tx;
			m_Vertexes[idx + 1].V = ty;
			m_Vertexes[idx + 1].Color = clr;

			m_Positions[idx + 2].X = x + m_TileW;
			m_Positions[idx + 2].Y = y;
			m_Positions[idx + 2].Z = 0.0f;
			m_Vertexes[idx + 2].U = tx + tw;
			m_Vertexes[idx + 2].V = ty;
			m_Vertexes[idx + 2].Color = clr;

			m_Positions[idx + 3].X = x + m_TileW;
			m_Positions[idx + 3].Y = y + m_TileH;
			m_Positions[idx + 3].Z = 0.0f;
			m_Vertexes[idx + 3].U = tx + tw;
			m_Vertexes[idx + 3].V = ty + th;
			m_Vertexes[idx + 3].Color = clr;

			currTile++;
		}
	}
}

CTile* CTileMap2D::GetTileIJ(u32 _i, u32 _j)
{
	if(_i >= m_SizeI || _j >= m_SizeJ)
		return 0;

	return &m_TileMap[_j*m_SizeI+_i];
}

CTile* CTileMap2D::GetTileXY(s32 _x, s32 _y)
{
	f32 gx = _x - m_Node->GetPosition().X * m_Node->GetScale().X;
	f32 gy = _y - m_Node->GetPosition().Y * m_Node->GetScale().Y;

	s32 ti = -1;
	s32 tj = -1;

	if(m_MapType == EMT_ORTHOGONAL)
	{
		ti = gx/(f32(m_TileW) * m_Node->GetScale().X);
		tj = gy/(f32(m_TileH) * m_Node->GetScale().Y);
	}
	else
	if(m_MapType == EMT_ISOMETRIC)
	{
		
	}
	else
	if(m_MapType == EMT_STAGGERED)
	{
		
	}
	else
	if(m_MapType == EMT_HEXAGONAL)
	{
		ti = gx/(f32(m_TileW) * m_Node->GetScale().X * 0.75f); 
		tj = gy/(f32(m_TileH) * m_Node->GetScale().Y);

		f32 modX = s32(gx) % s32(m_TileW * m_Node->GetScale().X * 0.75f); 
		f32 modY = s32(gy) % s32(m_TileH * m_Node->GetScale().Y);

		f32 halfH = m_TileH * m_Node->GetScale().Y * 0.5f;
		f32 quarW = m_TileW * m_Node->GetScale().X * 0.25f;
		f32 kWH = quarW / halfH;
		
		if (ti % 2) 
		{
			if (modY >= halfH)
			{ 
				if (modX + (modY - halfH) * kWH < quarW)
				{ 
					//printf("Type A0: ModX: %.3f ModY: %.3f\n", modX, modY);
					ti -= 1; 
				}
				else
				{
					//printf("Type A1: ModX: %.3f ModY: %.3f\n", modX, modY);
				}
			}
			else 
			{ 
				if (modX - (modY + halfH) * kWH < -quarW)
				{ 
					//printf("Type A0: ModX: %.3f ModY: %.3f\n", modX, modY);
					ti -= 1; 
				} 
				else
				{ 
					//printf("Type A2: ModX: %.3f ModY: %.3f\n", modX, modY);
					tj -= 1; 
				}
			}
		}
		else
		{
			if (modX + modY * kWH < quarW)
			{ 
				//printf("Type B1: ModX: %.3f ModY: %.3f\n", modX, modY);
				ti -= 1;
				tj -= 1;
			} 

			if (modX - modY * kWH < -quarW) 
			{ 
				//printf("Type B2: ModX: %.3f ModY: %.3f\n", modX, modY);
				tj -= 1; 
			} 
		}
	}

	if(ti < 0 || ti >= m_SizeI || tj < 0 || tj >= m_SizeJ)
		return 0;
	else
		return &m_TileMap[tj*m_SizeI+ti];
}