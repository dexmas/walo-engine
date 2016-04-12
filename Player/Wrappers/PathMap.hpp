#pragma once

#include "Core/Types.hpp"
#include "Core/Vector.hpp"
#include "Core/List.hpp"

#include <sqrat.h>

class CMapCell
{
public:
	u32 I;
	u32 J;
	
	CVector2 Pos;

	s32	Cost;

	bool Pass;
};

class CPathMap
{
public:
	CPathMap()
	{
		m_CellsCount = 0;
		m_Cells = 0;
	}

	~CPathMap()
	{
		delete[] m_Cells;
	}

	void Init(Sqrat::Object _w, Sqrat::Object _h)
    {
		Width = _w.Cast<u32>();
		Height = _h.Cast<u32>();

		if(m_Cells)
			delete[] m_Cells;

		m_CellsCount = Width*Height;
		m_Cells = new CMapCell[m_CellsCount];

		for(u32 i=0;i<Width;i++)
		{
			for(u32 j=0;j<Height;j++)
			{
				m_Cells[j * Width + i].I = i;
				m_Cells[j * Width + i].J = j;
				m_Cells[j * Width + i].Pass = true;
			}
		}
    }

	CMapCell* GetCell(s32 _i, s32 _j)
	{
		if(_j * Width + _i < m_CellsCount)
			return &m_Cells[_j * Width + _i];
		return 0;
	}

	bool GetPass(s32 _i, s32 _j)
	{
		if(_j * Width + _i < m_CellsCount)
			return m_Cells[_j * Width + _i].Pass;
		return false;
	}

	void SetPass(s32 _i, s32 _j, bool _pass)
	{
		if(_j * Width + _i < m_CellsCount)
			m_Cells[_j * Width + _i].Pass = _pass;
	}

    bool CalcPath(u32 StartI, u32 StartJ,u32 DestI,u32 DestJ, CList<CMapCell*>& Output)
    {
		if(StartI > Width || StartJ > Height || DestI > Width || DestJ > Height)
			return false;

        Output.Clear();
        
        CList<CMapCell*> tmpCells;
        
        CMapCell* begin = GetCell(StartI, StartJ);
        CMapCell* end   = GetCell(DestI,  DestJ);
        
        if(!begin || !end)
            return false;
        
        _ResetCellsCost(4096*4096);
        
        end->Cost = 0;
        
        tmpCells.PushBack(end);
        s32 walkTempl[8][2]={{1,0},{0,1},{-1,0},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
        
        bool Continue = true;
        
        while(Continue)
        {
            CMapCell* c = *(tmpCells.Begin());
            CList<CMapCell*>::CIterator it = tmpCells.Begin();
			tmpCells.Erase(it);
            
            for(u32 i=0;i<4;i++)
            {
                CMapCell* pc = GetCell(c->I+walkTempl[i][0],c->J+walkTempl[i][1]);
                if(pc)
                {
                    if(pc == begin)
                        Continue = false;
                    if(pc->Cost > (c->Cost + 1) && pc->Pass)
                    {
                        pc->Cost = c->Cost + 1;
                        tmpCells.PushBack(pc);
                    }
                }
            }

			if(Continue && !tmpCells.Size())
			{
				Output.Clear();
				return false;
			}
        }
        
        CMapCell* c = begin;
        
        while(c != end)
        {
            CMapCell* tc = 0;
            s32 pc_cost = c->Cost;
            
            for(u32 i=0;i<4;i++)
            {
                CMapCell* c1 = GetCell(c->I+walkTempl[i][0],c->J+walkTempl[i][1]);
                CMapCell* c2 = GetCell(c->I+walkTempl[i][0],c->J);
                CMapCell* c3 = GetCell(c->I,c->J+walkTempl[i][1]);
                
                if(c1 && c2 && c3)
                {
                    if((c1->Pass || c1 == end) && c2->Pass && c3->Pass)
                    {
                        if(c1->Cost < pc_cost)
                        {
                            tc = c1;
                            pc_cost = c1->Cost;
                        }
                    }
                }
            }
            
            for(u32 i=0;i<4;i++)
            {
                CMapCell* pc = GetCell(c->I+walkTempl[i][0],c->J+walkTempl[i][1]);
                if(pc)
                {
                    if((pc->Pass || pc == end) && pc->Cost < pc_cost)
                    {
                        tc = pc;
                        pc_cost = c->Cost;
                    }
                }
            }
            
            Output.PushBack(tc);
            c = tc;
        }
        
        return true; 
    }

	bool FindPlace(s32 _startI, s32 _startJ, u32 _sizeI, u32 _sizeJ)
	{
		return false;
	}
    
	bool _SQCalcPath(s32 StartI, s32 StartJ, s32 DestI, s32 DestJ, Sqrat::Object _path)
	{
		CList<CMapCell*> path;

		bool ret = CalcPath(StartI, StartJ, DestI, DestJ, path);

		CList<CMapCell*>::CIterator it = path.Begin();

		Sqrat::Array patharray(_path);

		while(it != path.End())
		{

			patharray.Append<CMapCell>(*it);

			++it;
		}

		return ret;
	}

	u32	Width;
	u32	Height;

	static void Register(HSQUIRRELVM vm)
	{
		{
		Sqrat::Class<CMapCell, Sqrat::NoCopy<CMapCell> > cl(vm);

		cl.Var("I", &CMapCell::I);
		cl.Var("J", &CMapCell::J);

		Sqrat::RootTable(vm).Bind("CPathMapCell", cl);
		}
		{
		Sqrat::Class<CPathMap, Sqrat::NoCopy<CPathMap> > cl(vm);

		cl.Func("Init", &CPathMap::Init);
		cl.Func("GetPass", &CPathMap::GetPass);
		cl.Func("SetPass", &CPathMap::SetPass);
		cl.Func("CalcPath", &CPathMap::_SQCalcPath);

		Sqrat::RootTable(vm).Bind("CPathMap", cl);
		}
	}

private:
	void _ResetCellsCost(s32 cost)
	{
		for(u32 i=0; i<Width*Height;i++)
			m_Cells[i].Cost = cost;
	}

	CMapCell*	m_Cells;
	u32			m_CellsCount;
};