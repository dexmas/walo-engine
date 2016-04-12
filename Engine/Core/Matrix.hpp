#pragma once

#include "Core/Vector.hpp"
#include "Core/BBox.hpp"
#include "Core/Rect.hpp"

#include <memory.h>

class CMatrix4
{
public:
	CMatrix4()
	{
		memset(M, 0, 16*sizeof(f32));
		M[0] = M[5] = M[10] = M[15] = 1.0f;
	}

	CMatrix4(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23, f32 m30, f32 m31, f32 m32, f32 m33)
	{
		M[0]  = m00; M[1]  = m01; M[2]  = m02; M[3]  = m03;
		M[4]  = m10; M[5]  = m11; M[6]  = m12; M[7]  = m13;
		M[8]  = m20; M[9]  = m21; M[10] = m22; M[11] = m23;
		M[12] = m30; M[13] = m31; M[14] = m32; M[15] = m33;
	}

	inline CMatrix4& Rotate(const CVector3& _rot)
	{
		const f32 cr = CMath::Cos(_rot.X);
		const f32 sr = CMath::Sin(_rot.X);
		const f32 cp = CMath::Cos(_rot.Y);
		const f32 sp = CMath::Sin(_rot.Y);
		const f32 cy = CMath::Cos(_rot.Z);
		const f32 sy = CMath::Sin(_rot.Z);

		M[0] = cp*cy;
		M[1] = cp*sy;
		M[2] = -sp;

		const f32 srsp = sr*sp;
		const f32 crsp = cr*sp;

		M[4] = srsp*cy-cr*sy;
		M[5] = srsp*sy+cr*cy;
		M[6] = sr*cp;

		M[8] = crsp*cy+sr*sy;
		M[9] = crsp*sy-sr*cy;
		M[10] = cr*cp;

		return *this;
	}

	inline CMatrix4& Translate(const CVector3& _pos)
	{
		M[12] = _pos.X;
		M[13] = _pos.Y;
		M[14] = _pos.Z;

		return *this;
	}

	inline CMatrix4& Scale(const CVector3& _scale)
	{
		M[0] =  _scale.X;
		M[5] =  _scale.Y;
		M[10] = _scale.Z;

		return *this;
	}

	inline CVector3 Rotation() const
	{
		CVector3 ret;

		return ret;
	}

	inline CVector3 Translation() const
	{
		CVector3 ret;

		return ret;
	}

	inline CVector3 Scale() const
	{
		f32 x = sqrtf(M[0] * M[0] + M[4] * M[4] + M[8]  * M[8]);
		f32 y = sqrtf(M[1] * M[1] + M[5] * M[5] + M[9]  * M[9]);
		f32 z = sqrtf(M[2] * M[2] + M[6] * M[6] + M[10] * M[10]);
		
		return CVector3(x, y, z);
	}

	inline CMatrix4 operator*(const f32 scalar) const
	{
		CMatrix4 temp;

		temp.M[0] = M[0]*scalar;
		temp.M[1] = M[1]*scalar;
		temp.M[2] = M[2]*scalar;
		temp.M[3] = M[3]*scalar;
		temp.M[4] = M[4]*scalar;
		temp.M[5] = M[5]*scalar;
		temp.M[6] = M[6]*scalar;
		temp.M[7] = M[7]*scalar;
		temp.M[8] = M[8]*scalar;
		temp.M[9] = M[9]*scalar;
		temp.M[10] = M[10]*scalar;
		temp.M[11] = M[11]*scalar;
		temp.M[12] = M[12]*scalar;
		temp.M[13] = M[13]*scalar;
		temp.M[14] = M[14]*scalar;
		temp.M[15] = M[15]*scalar;

		return temp;
	}

	inline CMatrix4& operator*=(const CMatrix4& _other)
	{
		CMatrix4 m = *this;
		return _multiplay(m, _other);
	}

	inline void TransformVector(CVector3& _vec) const
	{
		f32 vector[3];

		vector[0] = _vec.X*M[0] + _vec.Y*M[4] + _vec.Z*M[8] + M[12];
		vector[1] = _vec.X*M[1] + _vec.Y*M[5] + _vec.Z*M[9] + M[13];
		vector[2] = _vec.X*M[2] + _vec.Y*M[6] + _vec.Z*M[10] + M[14];

		_vec.X = vector[0];
		_vec.Y = vector[1];
		_vec.Z = vector[2];
	}

	inline void TransformVector(CVector2& _vec) const
	{
		f32 vector[2];

		vector[0] = _vec.X*M[0] + _vec.Y*M[4] + M[12];
		vector[1] = _vec.X*M[1] + _vec.Y*M[5] + M[13];

		_vec.X = vector[0];
		_vec.Y = vector[1];
	}

	inline void TransformAABBox(CBBox& _box) const
	{
		CVector3 center = _box.Center();
		CVector3 edge = _box.Size() / 2.0f;

		TransformVector(center);

		f32 x = CMath::Abs(M[0]) * edge.X + CMath::Abs(M[4]) * edge.Y + CMath::Abs(M[8]) * edge.Z;
        f32 y = CMath::Abs(M[1]) * edge.X + CMath::Abs(M[5]) * edge.Y + CMath::Abs(M[9]) * edge.Z;
        f32 z = CMath::Abs(M[2]) * edge.X + CMath::Abs(M[6]) * edge.Y + CMath::Abs(M[10]) * edge.Z;

		edge = CVector3(x, y, z);
    
		_box = CBBox(center - edge, center + edge);
	}

	inline void TransformAARect(CRect& _rect) const
	{
		CVector2 center = CVector2(_rect.X + _rect.Width/2.0f, _rect.Y + _rect.Height/2.0f);
		CVector2 edge = CVector2(_rect.Width/2.0f, _rect.Height/2.0f);

		TransformVector(center);

		f32 x = CMath::Abs(M[0]) * edge.X + CMath::Abs(M[4]) * edge.Y;
        f32 y = CMath::Abs(M[1]) * edge.X + CMath::Abs(M[5]) * edge.Y;

		edge = CVector2(x, y);
    
		_rect = CRect(s32((center - edge).X), s32((center - edge).Y), u32(edge.X * 2), u32(edge.Y * 2));
	}

	CMatrix4& Perspective(f32 _fov, f32 _ratio, f32 _near, f32 _far)
	{
		const f32 h = 1.0f/tanf(_fov*0.5f);
		const f32 w = h / _ratio;

		M[0] = w;
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = h;
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;

		M[10] = _far/(_near-_far);

		M[11] = -1;

		M[12] = 0;
		M[13] = 0;

		M[14] = _near*_far/(_near-_far);

		M[15] = 0;

		return *this;
	}

	CMatrix4& Ortho(u32 _width, u32 _height, f32 _near, f32 _far, bool _rh = false)
	{
		M[0] = 2.0f/f32(_width);
		M[1] = 0;
		M[2] = 0;
		M[3] = 0;

		M[4] = 0;
		M[5] = -2.0f/f32(_height);
		M[6] = 0;
		M[7] = 0;

		M[8] = 0;
		M[9] = 0;

		M[10] = 1.0f/(_far-_near);

		M[11] = 0;

		M[12] = -1;
		M[13] = 1;
		M[14] = _near/(_near-_far);

		M[15] = 1;

		return *this;
	}

	inline CMatrix4& LookAt(CVector3& _pos, CVector3& _target, CVector3& _up)
	{
		CVector3 zaxis = _pos - _target;
		zaxis.Normalize();

		CVector3 xaxis = _up.Cross(zaxis);
		xaxis.Normalize();

		CVector3 yaxis = zaxis.Cross(xaxis);

		M[0] = xaxis.X;
		M[1] = yaxis.X;
		M[2] = zaxis.X;
		M[3] = 0;

		M[4] = xaxis.Y;
		M[5] = yaxis.Y;
		M[6] = zaxis.Y;
		M[7] = 0;

		M[8] = xaxis.Z;
		M[9] = yaxis.Z;
		M[10] = zaxis.Z;
		M[11] = 0;

		M[12] = -xaxis.Dot(_pos);
		M[13] = -yaxis.Dot(_pos);
		M[14] = -zaxis.Dot(_pos);
		M[15] = 1;

		return *this;
	}

	CMatrix4 Transpose()
	{
		f32 temp = M[1];
		M[1] = M[4];
		M[4] = temp;

		temp = M[2];
		M[2] = M[8];
		M[8] = temp;

		temp = M[3];
		M[3] = M[12];
		M[12] = temp;

		temp = M[9];
		M[6] = M[9];
		M[9] = temp;

		temp = M[7];
		M[7] = M[13];
		M[13] = temp;

		temp = M[11];
		M[11] = M[14];
		M[14] = temp;

		return *this;
	}

	bool Inverse()
	{
		CMatrix4 m;

		f32 a00 = (M[5] * ((M[10] * M[15]) - (M[11] * M[14])) - M[6] * (M[9] * M[15] - M[11] * M[13]) + M[7] * (M[9] * M[14] - M[10] * M[13]));
		f32 a01 = -1 * (M[4] * ((M[10] * M[15]) - (M[11] * M[14])) - M[6] * (M[8] * M[15] - M[11] * M[12]) + M[7] * (M[8] * M[14] - M[10] * M[12]));
		f32 a02 = (M[4] * ((M[9] * M[15]) - (M[11] * M[13])) - M[5] * (M[8] * M[15] - M[11] * M[12]) + M[7] * (M[8] * M[13] - M[9] * M[12]));
		f32 a03 = -1 * (M[4] * ((M[9] * M[14]) - (M[10] * M[13])) - M[5] * (M[8] * M[14] - M[10] * M[12]) + M[6] * (M[8] * M[13] - M[9] * M[12]));
		f32 det = a00*M[0] + a01*M[1] + a02*M[2] + a03*M[3];

		if(det != (f32)0)
		{
			m.M[0] = a00;
			m.M[1] = a01;
			m.M[2] = a02;
			m.M[3] = a03;

			m.M[4] = -1 * (M[1] * ((M[10] * M[15]) - (M[11] * M[14])) - M[2] * ((M[9] * M[15]) - (M[11] * M[13])) + M[3] * ((M[9] * M[14]) - (M[10] * M[13])));
			m.M[5] = (M[0] * ((M[10] * M[15]) - (M[11] * M[14])) - M[2] * ((M[8] * M[15]) - (M[11] * M[12])) + M[3] * ((M[8] * M[14]) - (M[10] * M[12])));
			m.M[6] = -1 * (M[0] * ((M[9] * M[15]) - (M[11] * M[13])) - M[1] * ((M[8] * M[15]) - (M[11] * M[12])) + M[3] * ((M[8] * M[13]) - (M[9] * M[12])));
			m.M[7] = (M[0] * ((M[9] * M[14]) - (M[10] * M[13])) - M[1] * ((M[8] * M[14]) - (M[10] * M[12])) + M[2] * ((M[8] * M[13]) - (M[9] * M[12])));

			m.M[8] = (M[1] * ((M[6] * M[15]) - (M[7] * M[14])) - M[2] * ((M[5] * M[15]) - (M[7] * M[13])) + M[3] * ((M[5] * M[14]) - (M[6] * M[13])));
			m.M[9] = -1 * (M[0] * ((M[6] * M[15]) - (M[7] * M[14])) - M[2] * ((M[4] * M[15]) - (M[7] * M[12])) + M[3] * ((M[4] * M[14]) - (M[6] * M[12])));
			m.M[10] = (M[0] * ((M[5] * M[15]) - (M[7] * M[13])) - M[1] * ((M[4] * M[15]) - (M[7] * M[12])) + M[3] * ((M[4] * M[13]) - (M[5] * M[12])));
			m.M[11] = -1 * (M[0] * ((M[5] * M[14]) - (M[6] * M[13])) - M[1] * ((M[4] * M[14]) - (M[6] * M[12])) + M[2] * ((M[4] * M[13]) - (M[5] * M[12])));


			m.M[12] = -1 * (M[1] * ((M[6] * M[11]) - (M[7] * M[10])) - M[2] * ((M[5] * M[11]) - (M[7] * M[9])) + M[3] * ((M[5] * M[10]) - (M[6] * M[9])));
			m.M[13] = (M[0] * ((M[6] * M[11]) - (M[7] * M[10])) - M[2] * ((M[4] * M[11]) - (M[7] * M[8])) + M[3] * ((M[4] * M[10]) - (M[6] * M[8])));
			m.M[14] = -1 * (M[0] * ((M[5] * M[11]) - (M[7] * M[9])) - M[1] * ((M[4] * M[11]) - (M[7] * M[8])) + M[3] * ((M[4] * M[9]) - (M[5] * M[8])));
			m.M[15] = (M[0] * ((M[5] * M[10]) - (M[6] * M[9])) - M[1] * ((M[4] * M[10]) - (M[6] * M[8])) + M[2] * ((M[4] * M[9]) - (M[5] * M[8])));

			m.Transpose();

			det = (f32)1.0 / det;
			m = (m*det);

			*this = m;

			return true;
		}

		*this = m;

		return false;
	}

	f32* Pointer() 
	{ 
		return M;
	}

	f32 M[16];

	static CMatrix4 IDENTY;

private:
	inline CMatrix4& _multiplay(const CMatrix4& _a,const CMatrix4& _b )
	{
		const f32 *m1 = _a.M;
		const f32 *m2 = _b.M;

		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

		return *this;
	}
};