#pragma once

#include "Core/Types.hpp"
#include "Core/Math.hpp"

class CVector2
{
public:
    CVector2():X(0),Y(0){}
    CVector2(f32 _x, f32 _y):X(_x),Y(_y){}
	CVector2(const CVector2& _other): X(_other.X), Y(_other.Y){}

	inline CVector2 operator - () const
	{
		return CVector2(-X, -Y);
	}

	inline CVector2 operator = (const CVector2& _other) 
	{
		X = _other.X; 
		Y = _other.Y; 

		return *this; 
	}

	inline CVector2 operator + (const CVector2& _arg) const
	{
		return CVector2(X + _arg.X, Y + _arg.Y);
	}

	inline CVector2 operator - (const CVector2& _arg) const
	{
		return CVector2(X - _arg.X, Y - _arg.Y);
	}

	inline CVector2& operator += (const CVector2& _arg)
	{
		X += _arg.X;
		Y += _arg.Y;
		return *this;
	}

	inline CVector2& operator -= (const CVector2& _arg)
	{
		X -= _arg.X;
		Y -= _arg.Y;
		return *this;
	}

	inline CVector2 operator * (const f32 _arg) const
	{
		return CVector2(X * _arg, Y * _arg);
	}

	inline CVector2 operator / (const f32 _arg) const
	{
		return CVector2(X / _arg, Y / _arg);;
	}

	inline CVector2& operator *= (const f32 _arg)
	{
		X *= _arg;
		Y *= _arg;
		return *this;
	}

	inline CVector2& operator /= (const f32 _arg)
	{
		X *= _arg;
		Y *= _arg;
		return *this;
	}

	inline bool operator == (const CVector2& _arg)
	{
		if ((CMath::Abs(X - _arg.X) < CMath::EPS) &&
			(CMath::Abs(Y - _arg.Y) < CMath::EPS))
		{
			return true;
		}
		return false;
	}

	inline bool operator != (const CVector2& _arg)
	{
		return !(operator==(_arg));
	}

	f32 Dot(const CVector2& _other) const
	{
		return X * _other.X + Y * _other.Y;
	}

	inline void Normalize()
	{
		f32 fInvSquare = CMath::QRSqrt(X * X + Y * Y);
		X *= fInvSquare;
		Y *= fInvSquare;
	}

	inline f32 Length()
	{
		return 1.0f / CMath::QRSqrt(X * X + Y * Y);
	}

	inline f32 Distance(const CVector2& _arg)
	{
		return CVector2(X - _arg.X, Y - _arg.Y).Length();
	}

	inline void Interpolate(const CVector2& _one, const CVector2& _two, f32 _k)
	{
		X = _one.X + (_two.X - _one.X)*_k;
		Y = _one.Y + (_two.Y - _one.Y)*_k;
	}

    f32 X;
    f32 Y;
};

class CVector3
{
public:
    CVector3():X(0),Y(0),Z(0){}
    CVector3(f32 _x, f32 _y, f32 _z):X(_x),Y(_y),Z(_z){}
	CVector3(const CVector3& _other): X(_other.X), Y(_other.Y), Z(_other.Z){}

	inline CVector3 operator - (void) const
	{
		return CVector3(-X, -Y, -Z);
	}

	inline CVector3 operator + (const CVector3& _arg) const
	{
		return CVector3(X + _arg.X, Y + _arg.Y, Z + _arg.Z);
	}

	inline CVector3 operator - (const CVector3& _arg) const
	{
		return CVector3(X - _arg.X, Y - _arg.Y, Z - _arg.Z);
	}

	inline CVector3& operator += (const CVector3& _arg)
	{
		X += _arg.X;
		Y += _arg.Y;
		Z += _arg.Z;

		return *this;
	}

	inline CVector3& operator -= (const CVector3& _arg)
	{
		X -= _arg.X;
		Y -= _arg.Y;
		Z -= _arg.Z;

		return *this;
	}

	inline CVector3 operator * (const f32 _arg) const
	{
		return CVector3(X * _arg, Y * _arg, Z * _arg);
	}

	inline CVector3 operator / (const f32 _arg) const
	{
		return CVector3(X / _arg, Y / _arg, Z / _arg);
	}

	inline CVector3 operator *= (const f32 _arg)
	{
		X *= _arg;
		Y *= _arg;
		Z *= _arg;

		return *this;
	}

	inline CVector3 operator /= (const f32 _arg)
	{
		X /= _arg;
		Y /= _arg;
		Z /= _arg;

		return *this;
	}

	inline bool operator == (const CVector3& _arg)
	{
		if ((CMath::Abs(X - _arg.X) < CMath::EPS) &&
			(CMath::Abs(Y - _arg.Y) < CMath::EPS) &&
			(CMath::Abs(Z - _arg.Z) < CMath::EPS))
		{
			return true;
		}
		return false;
	}

	inline bool operator != (const CVector3& _arg)
	{
		return !(operator==(_arg));
	}

	inline void Normalize()
	{
		f32 fInvSquare = CMath::QRSqrt(X * X + Y * Y + Z * Z);

		X *= fInvSquare;
		Y *= fInvSquare;
		Z *= fInvSquare;
	}

	inline CVector3 Cross(const CVector3& _arg) const
	{
		return CVector3(Y * _arg.Z - Z * _arg.Y, Z * _arg.X - X * _arg.Z, X * _arg.Y - Y * _arg.X);
	}

	inline f32 Dot(const CVector3& _arg) const
	{
		return X * _arg.X + Y * _arg.Y + Z * _arg.Z;
	}

	inline void Interpolate(const CVector3& _one, const CVector3& _two, f32 _k)
	{
		X = _one.X + (_two.X - _one.X)*_k;
		Y = _one.Y + (_two.Y - _one.Y)*_k;
		Z = _one.Z + (_two.Z - _one.Z)*_k;
	}

    f32 X;
    f32 Y;
    f32 Z;
};