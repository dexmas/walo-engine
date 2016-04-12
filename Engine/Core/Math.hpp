#pragma once

#include <Core/Types.hpp>

#include <stdlib.h>
#include <math.h>

enum EIntersect
{
	EI_OUTSIDE = 0,
	EI_INTERSECT,
	EI_INSIDE
};

class CMath
{
public:
	static const f32 PI;
	static const f32 TWO_PI;
	static const f32 PI_DIV_TWO;
	static const f32 RADTODEG;
	static const f32 DEGTORAD;
	static const f32 EPS;

	// Integers

	inline static s32 Min(s32 a, s32 b)
	{
		return a < b ? a : b;
	}

	inline static s32 Max(s32 a, s32 b)
	{
		return a < b ? b : a;
	}

	inline static s32 Abs(s32 a)
	{
		return a < 0 ? -a : a;
	}

	inline static s32 Lerp(s32 a, s32 b, f32 t)
	{
		return s32(a*(1.f-t) + (b*t));
	}

	inline static s32 Clamp(s32 _value, s32 _low, s32 _high)
	{
		return Min(Max(_value,_low),_high);
	}

	inline static s32 Rand(s32 _from, s32 _to)
	{
		if((_to - _from) == 0)
			return _from;
		return _from + rand()%(_to - _from);
	}

	inline static u32 NextPower2(u32 _arg) 
	{
		u32 res = 1;
		while(res < _arg)
			res <<= 1;
		return res;
	}

	// Floats

	inline static f32 Min(f32 a, f32 b)
	{
		return a < b ? a : b;
	}

	inline static f32 Max(f32 a, f32 b)
	{
		return a < b ? b : a;
	}

	inline static f32 Abs(f32 a)
	{
		return a < 0 ? -a : a;
	}

	inline static f32 Lerp(f32 a, f32 b, f32 t)
	{
		return a*(1.f-t) + (b*t);
	}

	inline static f32 Clamp(f32 _value, f32 _low, f32 _high)
	{
		return Min(Max(_value,_low),_high);
	}

	inline static f32 Ceil(f32 _src)
	{
		return ceilf(_src);
	}

	inline static f32 Floor(f32 _x)
	{
		return floorf(_x);
	}

	inline static f32 Sqrt(f32 _arg)
	{
		return sqrtf(_arg);
	}

	inline static f32 QRSqrt(f32 _arg)
	{
		f32 x2 = _arg * 0.5f, y = _arg;
		u32 i  = * ( u32 * ) &y;					// evil floating point bit level hacking
		i  = 0x5f3759df - ( i >> 1 );				// what the fuck?
		y  = * ( f32 * ) &i;
		const f32 threehalfs = 1.5f;
		y = y * ( threehalfs - ( x2 * y * y ) );	// 1st iteration
		return y;
	}

	inline static float Sin(float angle)
	{
		return sinf(angle * DEGTORAD); 
	}

	inline static float Cos(float angle) 
	{ 
		return cosf(angle * DEGTORAD); 
	}

	inline static float Tan(float angle) 
	{ 
		return tanf(angle * DEGTORAD); 
	}

	inline static float Asin(float x) 
	{ 
		return RADTODEG * asinf(Clamp(x, -1.0f, 1.0f)); 
	}

	inline static float Acos(float x) 
	{ 
		return RADTODEG * acosf(Clamp(x, -1.0f, 1.0f)); 
	}

	inline static float Atan(float x) 
	{ 
		return RADTODEG * atanf(x); 
	}

	inline static float Atan2(float y, float x) 
	{ 
		return RADTODEG * atan2f(y, x); 
	}
};