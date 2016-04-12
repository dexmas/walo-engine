#pragma once

#include "Core/Types.hpp"
#include "Core/Vector.hpp"

class CRect
{
public:
	s32 X;
	s32 Y;
	s32 Width;
	s32 Height;

	CRect(){X = 0; Y = 0; Width = 0; Height = 0;}
	CRect(s32 _x, s32 _y, u32 _w, u32 _h){X = _x; Y = _y; Width = _w; Height = _h;}

	void Merge(const CVector2& _point)
	{
		if(_point.X < X)
			X = s32(_point.X);
		if(_point.Y < Y)
			Y = s32(_point.Y);
		if(_point.X > X + Width)
			Width = u32(_point.X - X);
		if(_point.Y > Y + Height)
			Height = u32(_point.Y - Y);
	}

	void Merge(const CRect& _other)
	{
		if(_other.X < X)
			X = _other.X;
		if(_other.Y < Y)
			Y = _other.Y;
		if(_other.X + _other.Width > X + Width)
			Width = _other.X + _other.Width - X;
		if(_other.Y + _other.Height > Y + Height)
			Height = _other.Y + _other.Height - Y;
	}

	bool operator==(const CRect& _other) const
	{
		return (X == _other.X) && (Y == _other.Y) && (Width == _other.Width) && (Height == _other.Height);
	}

	bool Intersect(const CRect& _other) const
	{
		return (X <= (_other.X + _other.Width) && Y <= (_other.Y + _other.Height) && (X + Width) >= _other.X && (Y + Height) >= _other.Y);
	}
};