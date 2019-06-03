#pragma once

#include "Node2DWrapper.hpp"
#include <Scene/Scene2D/TileMap2D.hpp>

class CTileMap2DWrapper: public CNode2DWrapper
{
public:
	CTileMap2DWrapper()
	{
		m_Object2D = new CTileMap2D();
		AddComponent(m_Object2D);
	}

	static void Register(HSQUIRRELVM _vm)
	{
		Sqrat::Class<CTile, Sqrat::NoConstructor > tcl(_vm);

		tcl.Var<u32>("ID", &CTile::ID);
		tcl.Var<s32>("I", &CTile::I);
		tcl.Var<s32>("J", &CTile::J);
		tcl.Var<f32>("X", &CTile::X);
		tcl.Var<f32>("Y", &CTile::Y);
		tcl.Var<f32>("H", &CTile::H);
		tcl.Var<u32>("Flags", &CTile::Flags);

		Sqrat::RootTable(_vm).Bind("CTile", tcl);

		Sqrat::DerivedClass<CTileMap2DWrapper, CNode2DWrapper, Sqrat::NoCopy<CTileMap2DWrapper> > cl(_vm);

		cl.Func("Init", &CTileMap2DWrapper::sqInit);
		cl.Func("GetTileXY", &CTileMap2DWrapper::sqGetTileXY);
		cl.Func("GetTileIJ", &CTileMap2DWrapper::sqGetTileIJ);
		cl.Func("RegisterTile", &CTileMap2DWrapper::sqRegisterTile);
		cl.Func("GetTileID", &CTileMap2DWrapper::sqGetTileID);
		cl.Func("SetTileID", &CTileMap2DWrapper::sqSetTileID);
		cl.Func("SetTileColor", &CTileMap2DWrapper::sqSetTileColor);
		cl.Func("SetTileFlags", &CTileMap2DWrapper::sqSetTileFlags);
		cl.Func("SetTexture", &CTileMap2DWrapper::sqSetTexture);
		cl.Func("CalcPath", &CTileMap2DWrapper::sqCalcPath);
		cl.Func("FindTiles", &CTileMap2DWrapper::sqFindTiles);
		
		Sqrat::RootTable(_vm).Bind("CTileMap2D", cl);
	}

	void sqInit(u32 _type, u32 _sizeI, u32 _sizeJ, u32 _tileW, u32 _tileH)
	{
		GetObject2D<CTileMap2D>()->Init((CTileMap2D::ETilemapType)_type, _sizeI, _sizeJ, _tileW, _tileH);
	}

	CTile* sqGetTileXY(s32 _x, s32 _y)
	{
		return GetObject2D<CTileMap2D>()->GetTileXY(_x, _y);
	}

	CTile* sqGetTileIJ(u32 _i, u32 _j)
	{
		return GetObject2D<CTileMap2D>()->GetTileIJ(_i, _j);
	}

	void sqRegisterTile(u32 _id, s32 _x, s32 _y, u32 _w, u32 _h)
	{
		GetObject2D<CTileMap2D>()->RegisterTile(_id, _x, _y, _w, _h);
	}

	u32 sqGetTileID(u32 _i, u32 _j)
	{
		return GetObject2D<CTileMap2D>()->GetTileID(_i, _j);
	}

	void sqSetTileID(u32 _i, u32 _j, u32 _id)
	{
		GetObject2D<CTileMap2D>()->SetTileID(_i, _j, _id);
	}

	void sqSetTileColor(u32 _i, u32 _j, u32 _color)
	{
		GetObject2D<CTileMap2D>()->SetTileColor(_i, _j, _color);
	}

	void sqSetTexture(CTexture* _tex)
	{
		GetObject2D<CTileMap2D>()->SetTexture(_tex);
	}

	void sqSetTileFlags(u32 _i, u32 _j, u32 _flags)
	{
		GetObject2D<CTileMap2D>()->SetTileFlags(_i, _j, _flags);
	}

	bool sqCalcPath(u32 _startI, u32 _startJ, u32 _destI, u32 _destJ, u32 _mask, Sqrat::Object _path)
	{
		CList<CTile*> path;

		bool ret = GetObject2D<CTileMap2D>()->CalcPath(_startI, _startJ, _destI, _destJ, _mask, path);

		CList<CTile*>::CIterator it = path.Begin();

		Sqrat::Array patharray(_path);

		while(it != path.End())
		{

			patharray.Append<CTile>(*it);

			++it;
		}

		return ret;
	}

	bool sqFindTiles(u32 _startI, u32 _startJ, u32 _steps, u32 _id, u32 _mask, Sqrat::Object _path)
	{
		CList<CTile*> path;

		u32 ret = GetObject2D<CTileMap2D>()->FindTiles(_startI, _startJ, _steps, _id, _mask, path);

		CList<CTile*>::CIterator it = path.Begin();

		Sqrat::Array patharray(_path);

		while (it != path.End())
		{

			patharray.Append<CTile>(*it);

			++it;
		}

		return ret;
	}
};