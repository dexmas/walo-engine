#pragma once

#include "Core/Math.hpp"
#include "Core/Vector.hpp"

class CPlane
{
public:
	CPlane()
	{
	}

	CPlane(const CVector3& _normal, f32 _distance)
	{
		Reset(_normal, _distance);
	}

	CPlane(const CVector3& _normal, const CVector3& _point)
	{
		Reset(_normal, _point);
	}

	CPlane(const CVector3& _p1, const CVector3& _p2, const CVector3& _p3)
	{
		Reset(_p1, _p2, _p3);
	}

	const CVector3& Normal() const {return m_Normal;}
	f32 Distance() const {return m_Distance;}

	void Reset(const CVector3& _normal, f32 _distance)
	{
		m_Normal = _normal;
		m_Distance = _distance;
	}

	void Reset(const CVector3& _normal, const CVector3& _point)
	{
		m_Normal = _normal;
		m_Distance = -_point.Dot(m_Normal);
	}

	void Reset(const CVector3& _p1, const CVector3& _p2, const CVector3& _p3)
	{
		m_Normal = (_p2 - _p1).Cross(_p3 - _p1);
		m_Normal.Normalize();
		m_Distance = -_p1.Dot(m_Normal);
	}

private:
	CVector3 m_Normal;
	f32		 m_Distance;
};