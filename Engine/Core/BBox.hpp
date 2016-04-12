#pragma once

#include "Core/Vector.hpp"
#include "Core/Plane.hpp"

class CBBox
{
public:
	CBBox()
	{
		m_MinEdge = CVector3(-1,-1,-1);
		m_MaxEdge = CVector3(1,1,1);
	}

	CBBox(const CVector3& _min, const CVector3& _max)
	{
		Reset(_min, _max);
	}

	CBBox(const CVector3& _init)
	{
		Reset(_init);
	}

	const CVector3& Min() const {return m_MinEdge;}
	const CVector3& Max() const {return m_MaxEdge;}

	void Reset(const CVector3& _min, const CVector3& _max)
	{
		m_MinEdge = _min;
		m_MaxEdge = _max;
	}

	void Reset(const CVector3& _init)
	{
		m_MinEdge = _init;
		m_MaxEdge = _init;
	}

	void AddPoint(const CVector3& _p)
	{
		if(_p.X > m_MaxEdge.X) m_MaxEdge.X = _p.X;
		if(_p.Y > m_MaxEdge.Y) m_MaxEdge.Y = _p.Y;
		if(_p.Z > m_MaxEdge.Z) m_MaxEdge.Z = _p.Z;

		if(_p.X < m_MinEdge.X) m_MinEdge.X = _p.X;
		if(_p.Y < m_MinEdge.Y) m_MinEdge.Y = _p.Y;
		if(_p.Z < m_MinEdge.Z) m_MinEdge.Z = _p.Z;
	}

	void AddBBox(const CBBox& _b)
	{
		AddPoint(_b.m_MaxEdge);
		AddPoint(_b.m_MinEdge);
	}

	CVector3 Center() const
	{
		return (m_MinEdge + m_MaxEdge) / 2.0f;
	}

	CVector3 Size() const
	{
		return m_MaxEdge - m_MinEdge;
	}

	bool Inside(const CVector3& _p) const
	{
		return (_p.X >= m_MinEdge.X && _p.X <= m_MaxEdge.X &&
				_p.Y >= m_MinEdge.Y && _p.Y <= m_MaxEdge.Y &&
				_p.Z >= m_MinEdge.Z && _p.Z <= m_MaxEdge.Z);
	}

	bool Inside(const CBBox& _other) const
	{
		return (m_MinEdge.X >= _other.m_MinEdge.X && m_MinEdge.Y >= _other.m_MinEdge.Y && m_MinEdge.Z >= _other.m_MinEdge.Z &&
			    m_MaxEdge.X <= _other.m_MaxEdge.X && m_MaxEdge.Y <= _other.m_MaxEdge.Y && m_MaxEdge.Z <= _other.m_MaxEdge.Z);
	}

	bool Intersect(const CBBox& _other) const
	{
		return (m_MinEdge.X <= _other.m_MaxEdge.X && m_MinEdge.Y <= _other.m_MaxEdge.Y && m_MinEdge.Z <= _other.m_MaxEdge.Z &&
			    m_MaxEdge.X >= _other.m_MinEdge.X && m_MaxEdge.Y >= _other.m_MinEdge.Y && m_MaxEdge.Z >= _other.m_MinEdge.Z);
	}

	/*bool Intersect(const CLine3& _line) const
	{
		return Intersect(_line.Middle(), _line.Vector().normalize(), (f32)(_line.Length() * 0.5));
	}*/

	bool Intersect(const CVector3& _linemiddle, const CVector3& _linevect, f32 _halflength) const
	{
		const CVector3 e = Size() * 0.5f;
		const CVector3 t = Center() - _linemiddle;

		if ((fabsf(t.X) > e.X + _halflength * fabsf(_linevect.X)) ||
			(fabsf(t.Y) > e.Y + _halflength * fabsf(_linevect.Y)) ||
			(fabsf(t.Z) > e.Z + _halflength * fabsf(_linevect.Z)) )
			return false;

		f32 r = e.Y * fabsf(_linevect.Z) + e.Z * fabsf(_linevect.Y);
		if(fabsf(t.Y * _linevect.Z - t.Z * _linevect.Y) > r )
			return false;

		r = e.X * fabsf(_linevect.Z) + e.Z * fabsf(_linevect.X);
		if(fabsf(t.Z * _linevect.X - t.X * _linevect.Z) > r )
			return false;

		r = e.X * fabsf(_linevect.Y) + e.Y * fabsf(_linevect.X);
		if (fabsf(t.X * _linevect.Y - t.Y * _linevect.X) > r)
			return false;

		return true;
	}

	EIntersect Intersect(CPlane& _plane) const
	{
		CVector3 nearPoint(m_MaxEdge);
		CVector3 farPoint(m_MinEdge);

		if(_plane.Normal().X > 0.0f)
		{
			nearPoint.X = m_MinEdge.X;
			farPoint.X = m_MaxEdge.X;
		}

		if(_plane.Normal().Y > 0.0f)
		{
			nearPoint.Y = m_MinEdge.Y;
			farPoint.Y = m_MaxEdge.Y;
		}

		if(_plane.Normal().Z > 0.0f)
		{
			nearPoint.Z = m_MinEdge.Z;
			farPoint.Z = m_MaxEdge.Z;
		}

		if(_plane.Normal().Dot(nearPoint) + _plane.Distance() > 0.0f)
			return EI_INSIDE;

		if(_plane.Normal().Dot(farPoint) + _plane.Distance() > 0.0f)
			return EI_INTERSECT;

		return EI_OUTSIDE;
	}

private:
	CVector3 m_MinEdge;
	CVector3 m_MaxEdge;
};