#pragma once

#include "Scene/Component.hpp"
#include "Core/Plane.hpp"
#include "Core/BBox.hpp"
#include "Core/Matrix.hpp"

class CNode;
class CScene3D;
class CSystem;

enum EFrustumPlane
{
	EFP_NEAR = 0,
	EFP_FAR,
	EFP_LEFT,
	EFP_RIGHT,
	EFP_TOP,
	EFP_BOTTOM,

	EFP_COUNT
};

class CViewFrustum
{
public:
	EIntersect Intersect(const CBBox& _box) const
	{
		CVector3 center = _box.Center();
        CVector3 edge = center - _box.Min();
        bool allInside = true;
        
        for(u32 i = 0; i < EFP_COUNT; i++)
        {
            const CPlane& plane = m_Planes[i];
            f32 dist = plane.Normal().Dot(center) + plane.Distance();
			CVector3 absnormal(CMath::Abs(plane.Normal().X), CMath::Abs(plane.Normal().Y), CMath::Abs(plane.Normal().Z));
            f32 absDist = absnormal.Dot(edge);
            
            if(dist < -absDist)
                return EI_OUTSIDE;
            else 
			if(dist < absDist)
                allInside = false;
        }
        
        return allInside ? EI_INSIDE : EI_INTERSECT;
	}

	void Setup(const CMatrix4& _view)
	{
		m_Planes[EFP_LEFT].Reset(CVector3(_view.M[3 ] + _view.M[0], _view.M[7 ] + _view.M[4], _view.M[11] + _view.M[8]), _view.M[15] + _view.M[12]);
		m_Planes[EFP_RIGHT].Reset(CVector3(_view.M[3 ] - _view.M[0], _view.M[7 ] - _view.M[4], _view.M[11] - _view.M[8]), _view.M[15] - _view.M[12]);
		m_Planes[EFP_TOP].Reset(CVector3(_view.M[3 ] - _view.M[1], _view.M[7 ] - _view.M[5], _view.M[11] - _view.M[9]), _view.M[15] - _view.M[13]);
		m_Planes[EFP_BOTTOM].Reset(CVector3(_view.M[3 ] + _view.M[1], _view.M[7 ] + _view.M[5], _view.M[11] + _view.M[9]), _view.M[15] + _view.M[13]);
		m_Planes[EFP_FAR].Reset(CVector3(_view.M[3 ] - _view.M[2], _view.M[7 ] - _view.M[6], _view.M[11] - _view.M[10]), _view.M[15] - _view.M[14]);
		m_Planes[EFP_NEAR].Reset(CVector3(_view.M[3 ] + _view.M[2], _view.M[7 ] + _view.M[6], _view.M[11] + _view.M[10]), _view.M[15] + _view.M[14]);

		for(u32 i=0; i != EFP_COUNT; i++)
		{
			const CVector3& nvec = m_Planes[i].Normal();
			const f32 dst = m_Planes[i].Distance();
			const f32 len = sqrtf(nvec.X * nvec.X + nvec.Y * nvec.Y + nvec.Z * nvec.Z);

			m_Planes[i].Reset(nvec/len, dst/len);
		}
	}

private:
	CPlane m_Planes[EFP_COUNT];
};

class CCamera3D: public CComponentImpl<CScene3D>
{
public:
	CCamera3D();
	virtual ~CCamera3D() {}

	void OnEnabled(bool _enabled);
	void OnTransformed();

	const CMatrix4&		GetProjection() const {return m_Projection;}
	const CMatrix4&		GetView() const {return m_View;}
	const CViewFrustum&	GetFrustum() const {return m_Frustum;}

private:
	CMatrix4	 m_Projection;
	CMatrix4	 m_View;
	CViewFrustum m_Frustum;
};